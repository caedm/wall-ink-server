#!/usr/bin/python3

#script to check the wall-ink device status'
#check their voltage levels and also if they have timed out

#import the mysql-conector library 
#(you will have to install mysql-connector before running this
#you can use `pip install mysql-connector`)
import mysql.connector

#import the json library
import json
#other builtin libraries
from datetime import datetime, timedelta, date
import time
import io
import sys
#urllib library for requesting the api
import urllib.request
import urllib.parse
import urllib.error

#library for calling shell stuff
import subprocess


#if you want debug output or not
DEBUG_MODE = False
#DEBUG_MODE = True

#if you want to run the Database testing/debug code for the monitoring database
TEST_DB = False
#TEST_DB = True

#flag to check if the python is more recent (for subprocess module)
RECENT_PYTHON=False

#### CHECK THE PYTHON VERSION ####
#check for 3.7
if((sys.version_info[0] == 3) and (sys.version_info[1] >= 7)):
    RECENT_PYTHON = True
else:
    #if not, then set flag to false so we can use backwards compatible code
    RECENT_PYTHON = False
#### CHECK IS DONE ####    

#info for the database
deviceDatabaseServer="db.yourserver.net"
deviceDatabaseUsername="devicedbusername"
deviceDatabasePassword="devicedbpassword"
deviceDatabaseName="devicedbname"
#table names in the database
deviceTableName="devices"
monitoringTableName="monitoring"

#base url for the api to get a resource name from id
resource_name_api_base_url="https://your-wall-ink-server.net/get_resource_name.php"

#name of the data tool executable
data_tool_exec="./wall_ink_data_tool.bin"
#the firmware name to send to the server in image requests
#('no_update' will prevent overwriting device check in data)
script_firmware="no_update"
#test mac address
test_mac_address="AAAABBBBCCCC"
#test error value
test_error=0
#test voltage value
test_voltage=4.00
#the display dimensions to put in the test query
test_width=640
test_height=384

#base url for the the server api that gives the image data
get_image_url="http://your-wall-ink-server.net/get_image.php"

#the image key to use in the data verification
default_image_key="hunter2"

# Voltage threshold to cause warning
warn_voltage=2.8

# Voltage threshold to cause critical
critical_voltage=2.6

# Check-in missed count threshold to cause warning
warning_missed = 2
# Check-in missed count threshold to cause critical
critical_missed = 4

#buffer for before/after check in times
minute_buffer = 2

#error flag for missed_count, to use if there is a data/server error
#set to a very high value
MISSED_ERROR_FLAG = 999
#string to use to pass an error from a function
ERROR_STR = "ERROR"

#class for the nagios return code levels
class Level:
    OK, WARNING, CRITICAL, UNKNOWN = range(0, 4)

    @staticmethod
    def str(level):
        return ["OK", "WARNING", "CRITICAL", "UNKNOWN"][level]


#function to print data
def print_device(device):
    """prints a string with some basic device data"""
    print(" * {id}, Mac addr: {mac} Voltage: {voltage} last Checked In: {lastCheckedIn}".format(
        id=device['device_id'], mac=device['mac_address'], voltage=device['voltage'], 
        lastCheckedIn=device['last_checked_in']))


def get_mac_addr_str(devices):
    """ gives a comma-separated string of devices' mac addresses """
    out_str = "["
    for device in devices:
        out_str += " " + device['mac_address'] + ","
    out_str += "]"
    return out_str

#function that takes gets the resource name by accessing the api on the server
def get_device_resource_name(device):
    """function that takes gets the resource name for a devic
    by accessing the api on the server"""
    #get values from device
    plugin_id = device['plugin']
    resource_id = device['resource_id']
    #setup data
    data_vals = { 'plugin_id' : plugin_id, 'resource_id' : resource_id }
    data = urllib.parse.urlencode(data_vals)
    url = resource_name_api_base_url + "?%s" % data
    #send the http request
    req = urllib.request.Request(url)
    #get response, handle any HTTP errors
    try: 
        response = urllib.request.urlopen(req)
    except urllib.error.HTTPError as e: 
        if DEBUG_MODE:
            print("Resource API HTTP error: ", e.code)
        return "RES_ERROR"
    except urllib.error.URLError as e:
        if DEBUG_MODE:
            print("Failed to reach Resource API server: ", e.reason)
        return "RES_ERROR"
    else:
        #if no HTTP errors
        #get the json from the response
        res_json = response.read()
        #check for errors in getting the JSON
        if RECENT_PYTHON:
            try:
                #convert the json to data
                res_data = json.loads(res_json)
            except json.JSONDecodeError as e:
                #report if an error getting the json
                if DEBUG_MODE:
                    #print the error
                    print("JSONDecodeError: " + e.msg + " in: " + e.doc)
                return "RES_ERROR"
            else:
                #get the name
                resource_name = res_data['resource_name']
                #return it 
                return resource_name
        else:
            try:
                #convert the json to data
                res_data = json.loads(res_json.decode('ascii'))
            except:
                #report if an error getting the json
                if DEBUG_MODE:
                    #print the error
                    print("JSON error!")
                return "RES_ERROR"
            else:
                #get the name
                resource_name = res_data['resource_name']
                #return it 
                return resource_name


#get a string with  mac address, resource info, and notes for a device
def get_basic_device_str(device):
    """get a string with  mac address, resource info, 
    notes, and missed check-in count for a device"""
    #check if it has a missed_count key or not first 
    #as not all queries will have it
    if 'missed_count' in device:
        #the string to return
        out_str = ("[ MAC addr: {mac}, Plugin: {plugin},"
                " Resource: {resource}, Notes: {notes},"
                " Missed Check-ins: {missed}]"
                ).format(
                mac=device['mac_address'],
                plugin=device['plugin'],
                resource=get_device_resource_name(device), 
                notes=device['notes'],
                missed=device['missed_count'])
    else:
        #the string to return, without missed_count
        out_str = ("[ MAC addr: {mac}, Plugin: {plugin},"
                " Resource: {resource}, Notes: {notes} ]"
                ).format(
                mac=device['mac_address'],
                plugin=device['plugin'],
                resource=get_device_resource_name(device), 
                notes=device['notes'])
    #return the string
    return out_str

def get_device_list_str(device_list):
    """generates a string with a list of devices and their info"""
    out_str = "{ "
    for device in device_list:
        out_str += get_basic_device_str(device) + ", "
    out_str += "}"
    return out_str

def get_timestamp_data(mac_addr):
    """ requests an image from the server for given mac_address,
    extracts the time data and returns it. """
    #setup values to pass
    data_vals = { 'mac_address' : mac_addr, 
                    'firmware' : script_firmware, 
                    'error' : test_error, 
                    'voltage' : test_voltage,
                    'width' : test_width, 
                    'height': test_height }
    #encode for url
    data = urllib.parse.urlencode(data_vals)
    #add to end of url for request
    url = get_image_url + "?%s" % data
    #send the request
    req = urllib.request.Request(url)
    #get response and check for errors
    try:
        response = urllib.request.urlopen(req)
    except urllib.error.HTTPError as e: 
        if DEBUG_MODE:
            print("Server HTTP error: ", e.code)
        return ERROR_STR
    except urllib.error.URLError as e:
        if DEBUG_MODE:
            print("Failed to reach server: ", e.reason)
        return ERROR_STR
    else:
        #if no errors
        #get the data
        image_data = response.read()
        #variable to store the process and output
        process = None
        output = ""

        if RECENT_PYTHON:
            #call the data tool, pass all the necessary args and pipe in the data
            process = subprocess.run([ data_tool_exec, 
                                        "-C", "-W", "-j", 
                                        "-i", default_image_key,
                                        "-m", mac_addr ], 
                                        capture_output=True,
                                        input=image_data)
            #check the return code
            if(process.returncode > 0):
                #if it was an error code (not 0) then report it
                if DEBUG_MODE:
                    print("Data Verification error! MAC Addr: ", mac_addr)
                    print("stderr output: ", process.stderr.decode('ascii')) 
                return ERROR_STR
            #store the output
            output  = process.stdout.decode('ascii')
        else:
            #if older version
            try:
                #call the data tool, pass all the necessary args and pipe in the data
                process = subprocess.check_output([ data_tool_exec, 
                                        "-C", "-W", "-j", 
                                        "-i", default_image_key,
                                        "-m", mac_addr ], 
                                        stderr=subprocess.STDOUT,
                                        input=image_data)
            except subprocess.CalledProcessError as e:
                #if it had errors
                if DEBUG_MODE:
                    print("Data Verification error! MAC Addr: ", mac_addr)
                    print("error output: ", e.output.decode('ascii'))
                #return an error 
                return ERROR_STR
            else:
                #if not error
                #get the output
                output = process.decode('ascii')

        #after checks
        #get values from json
        output_json_data = json.loads(output)
        #return the data
        return output_json_data


def get_human_time(unix_time):
    """returns a date time string of an input UNIX/POSIX time stamp"""
    time_data = datetime.fromtimestamp(unix_time)
    return str(time_data)


#connect to database
wallinkDB = mysql.connector.connect(
    host= deviceDatabaseServer,
    user= deviceDatabaseUsername,
    passwd= deviceDatabasePassword,
    database= deviceDatabaseName,
)
#create a cursor
mycursor = wallinkDB.cursor(dictionary=True)

#get all devices in production
mycursor.execute("SELECT * FROM " + deviceTableName 
        + " WHERE is_production IS TRUE")
#put into production dictionary
production = mycursor.fetchall()

if DEBUG_MODE:
    print("there are {} devices in production".format(
        str(mycursor.rowcount)))
    for row in production:
        print_device(row)
        print(get_device_resource_name(row))


#### BEGIN VOLTAGE CHECKING CODE ####

#check amount of devices under the voltage threshold
mycursor.execute("SELECT * FROM " + deviceTableName + 
        " WHERE is_production IS TRUE "
        + "AND voltage <= " + str(critical_voltage))

#get the critical voltage devices result
critical_result = mycursor.fetchall()
#get the count
critical_count = mycursor.rowcount

if DEBUG_MODE:
    print("critical count: " + str(critical_count))
    for row in critical_result:
        print_device(row)

#get warning level devices
mycursor.execute("SELECT * FROM " + deviceTableName + 
        " WHERE is_production IS TRUE" +
        " AND voltage <= " + str(warn_voltage)
        + " AND voltage > " + str(critical_voltage))
#get the result of which are at warning voltage level
warning_result = mycursor.fetchall()
#get the count
warning_count = mycursor.rowcount

if DEBUG_MODE:
    print("warning count: " + str(warning_count))
    for row in warning_result:
        print_device(row)

if DEBUG_MODE:
    #get current time
    current_time = datetime.now()
    
    print("current time: " + str(current_time)) 
    print("current time - buffer " + str(current_time - timedelta(minutes=minute_buffer)))

#### END OF VOLTAGE CHECKING CODE ###

def print_result(sql_result):
    """ prints each row's data in an sql query result """
    for row in sql_result:
        print(row)

# get the data to check for timeouts

def populate_monitoring_db(db):
    """populates the 'monitoring' database table with any
    new devices from the 'devices' database table"""
    #make a cursor
    db_cursor = db.cursor(dictionary=True)
    #populate db with the mac addresses
    #only add new ones that are not already in the database
    populate_query = (" INSERT INTO {monitoringTable} (mac_address) "
                    " SELECT mac_address FROM {deviceTable} "
                    " WHERE is_production IS TRUE "
                    " AND "
                    " NOT EXISTS "
                    " (SELECT mac_address FROM {monitoringTable} " 
                    " WHERE mac_address = {deviceTable}.mac_address); "
                    ).format(deviceTable=deviceTableName, 
                            monitoringTable=monitoringTableName)
    #execute populate
    db_cursor.execute(populate_query)
    #commit
    db.commit()
    #print for debug
    if TEST_DB:
        #execute listing
        db_cursor.execute(" SELECT * FROM {monitoringTable} ;".format(
            monitoringTable=monitoringTableName))
        #get result 
        result = db_cursor.fetchall()
        count = db_cursor.rowcount
        print("after populate: ")
        print_result(result)

def cleanup_monitoring_db(db): 
    """cleans out any devices in the 'monitoring' database table
    that are no longer in the 'devices' database table""" 
    #make a cursor
    db_cursor = db.cursor(dictionary=True)
    #delete non-used entires, where there is no equivalent 
    #mac address in the device table
    cleanup_query = (" DELETE FROM {monitoringTable} "
            " WHERE NOT EXISTS "
            " (SELECT mac_address FROM {deviceTable} "
            " WHERE is_production IS TRUE "
            " AND mac_address = {monitoringTable}.mac_address); "
            ).format( monitoringTable=monitoringTableName,
                    deviceTable=deviceTableName )
    #run cleanup query
    db_cursor.execute(cleanup_query)
    #commit
    db.commit()
    #print for debug
    if TEST_DB:
        #execute listing
        db_cursor.execute(" SELECT * FROM {monitoringTable} ;".format(
            monitoringTable=monitoringTableName))
        #get result 
        result = db_cursor.fetchall()
        count = db_cursor.rowcount
        print("after cleanup: ")
        print_result(result)
    
def print_joined_tables(db): 
    """prints out the joined table data 
    of the 'devices' and 'monitoring' tables"""
    #make a cursor
    db_cursor = db.cursor(dictionary=True)
    
    #query joins needed data from both the devices table and monitoring table    
    join_data_query = (" SELECT {monitoringTable}.mac_address, " 
            " {deviceTable}.resource_id, {deviceTable}.plugin,  "
            " {deviceTable}.notes, {deviceTable}.voltage, "
            " {deviceTable}.last_checked_in, "
            " {monitoringTable}.next_check_in, {monitoringTable}.missed_count  "
            " FROM {deviceTable} "
            " INNER JOIN {monitoringTable} ON "
            " {deviceTable}.mac_address = {monitoringTable}.mac_address ; "
            ).format(monitoringTable=monitoringTableName, 
                    deviceTable=deviceTableName)
    #execute join
    db_cursor.execute(join_data_query)
    #get result
    join_result = db_cursor.fetchall()
    #print it out
    #print("joined tables data:")
    print_result(join_result)
    
    
#function to check for timeouts and update the DB data with it
def update_and_check_times(db):
    """checks each device for any missed check-ins and updates
    the values in the 'monitoring' table based on the results"""
    #make a cursor
    db_cursor = db.cursor(dictionary=True)
    #query to get data from both tables,
    device_data_query = (" SELECT {monitoringTable}.mac_address, " 
            " {deviceTable}.resource_id, {deviceTable}.plugin,  "
            " {deviceTable}.notes, {deviceTable}.voltage, "
            " {deviceTable}.last_checked_in, "
            " {monitoringTable}.next_check_in, {monitoringTable}.missed_count "
            " FROM {deviceTable} "
            " INNER JOIN {monitoringTable} ON "
            " {deviceTable}.mac_address = {monitoringTable}.mac_address ; "
            ).format(monitoringTable=monitoringTableName, 
                    deviceTable=deviceTableName)
    #get device info needed
    db_cursor.execute(device_data_query)
    #put into dictionary
    devices = db_cursor.fetchall()

    #template query for updating the table values 
    update_query = (" UPDATE {monitoringTable} "
            " SET next_check_in = %(next_time)s, missed_count = %(missed)s"
            " WHERE mac_address = %(mac)s ;").format(
                    monitoringTable=monitoringTableName) 

    #create a datetime buffer variable
    buffer_time = timedelta(minutes=minute_buffer)

    #go through the devices
    for device in devices:
        
        #get the device image time data
        timestamp_data = get_timestamp_data(device['mac_address'])
        #check for error
        if(timestamp_data == ERROR_STR):
            if DEBUG_MODE:
                #print error
                print("error getting timestamp of device with MAC address: ", 
                        device['mac_address'])
            #set error flag in the missed count
            db_cursor.execute(update_query, { 
                'next_time': device['next_check_in'], 
                'missed': MISSED_ERROR_FLAG,
                'mac': device['mac_address'] })
            #go to next in loop and skip the rest of the code in this iteration
            continue

        #make datetime variables with the timestamps
        comp_datetime = datetime.fromtimestamp(timestamp_data['comp_time'])
        wake_datetime = datetime.fromtimestamp(timestamp_data['wake_time'])
        #
        #if no errors before this point, then the device is OK
        #so if it has an error flag, reset it before anything else
        if(device['missed_count'] >= MISSED_ERROR_FLAG):
            #reset the missed count
            db_cursor.execute(update_query, { 
                'next_time': device['next_check_in'], 
                'missed': 0,
                'mac': device['mac_address'] })
        

        #check if the table row is new and doesn't have a next_check_in value yet
        #if it doesn't, fix that
        #also fix if set with the error flag
        if(device['next_check_in'] == None):
            #update the value with the one from the timestamp
            db_cursor.execute(update_query, { 
                'next_time': wake_datetime, 
                'missed': 0,
                'mac': device['mac_address'] })
        #check if the wake time is ahead of the next check in time in the database
        #add buffer so the new time must be more than the buffer ahead of the previous
        #also, make sure our buffer time has passed before registering a new check in time
        #in order prevent race conditions
        elif (((wake_datetime - buffer_time) > device['next_check_in'])
                and ((comp_datetime - buffer_time) > device['next_check_in'])):

            #debug
            if DEBUG_MODE:
                print(("device: {} "
                        "Received a new check in time: {}, old time {}").format(
                    device['mac_address'], wake_datetime, device['next_check_in']))

            #the check in time has past, store it
            old_next_check_in = device['next_check_in']
            #check if the device checked in after the previous check in time
            #subtract the buffer minutes in the check
            if(device['last_checked_in'] >= (old_next_check_in - buffer_time)):
                #debug
                if DEBUG_MODE:
                    print(("device: {} "
                            "checked in within time: old {} - new {},"
                            " at time: {}").format(
                        device['mac_address'], device['next_check_in'], 
                        wake_datetime, device['last_checked_in']))

                #if it did then it is OK
                #update the data with the new next check in time and ZERO missed
                #execute query
                db_cursor.execute(update_query, { 
                    'next_time': wake_datetime, 
                    'missed': 0,
                    'mac': device['mac_address'] })
            else:
                #debug
                if DEBUG_MODE:
                    print(("device: {} "
                            "failed to checked in within time: old {} - new {},"
                            " at time: {}").format(
                        device['mac_address'], device['next_check_in'], 
                        wake_datetime, device['last_checked_in']))
                    print("incrementing missed count")

                #if not, then it has missed the check in time
                #update the data, increment the missed count and next check in time
                #make sure to not increment a flag value
                #execute query
                db_cursor.execute(update_query, { 
                    'next_time': wake_datetime, 
                    'missed': ((device['missed_count'] + 1) 
                        if (device['missed_count'] < MISSED_ERROR_FLAG) else 1),
                    'mac': device['mac_address'] } )

    #commit the changes to the database
    db.commit()
    #print debug info if needed
    if TEST_DB:
        print("joined tables: after updating")
        print_joined_tables(db)

        
#function to check for critical and warning level timeouts
def get_timeout_devices(db):
    """ function to go check for timed out devices
    in the monitoring table and return their data in separate lists """
    
    #lists to return at the end
    warning_devices = []
    critical_devices = []
    error_devices = [] 

    #make a cursor
    db_cursor = db.cursor(dictionary=True)
    #query to get data from both tables,
    #also just those with missed count > 0
    device_data_query = (" SELECT {monitoringTable}.mac_address, " 
            " {deviceTable}.resource_id, {deviceTable}.plugin,  "
            " {deviceTable}.notes, {deviceTable}.voltage, "
            " {deviceTable}.last_checked_in, "
            " {monitoringTable}.next_check_in, {monitoringTable}.missed_count "
            " FROM {deviceTable} "
            " INNER JOIN {monitoringTable} ON "
            " {deviceTable}.mac_address = {monitoringTable}.mac_address "
            " AND {monitoringTable}.missed_count > 0 ; "
            ).format(monitoringTable=monitoringTableName, 
                    deviceTable=deviceTableName)
    #get device info needed
    db_cursor.execute(device_data_query)
    #put into dictionary
    devices = db_cursor.fetchall()
    #template query for updating the table values 
    update_query = (" UPDATE {monitoringTable} "
            " SET next_check_in = %(next_time)s, missed_count = %(missed)s"
            " WHERE mac_address = %(mac)s ;").format(
                    monitoringTable=monitoringTableName) 

    if TEST_DB:
        #debug
        print("after timeout query:")
        print_result(devices)

    #loop through and categorize
    for device in devices:
        #check for errors first
        if(device['missed_count'] == MISSED_ERROR_FLAG): 
            #if it had an error, add to that list
            error_devices.append(device)
        elif(device['missed_count'] >= critical_missed):
            #check for critical timeout 
            #critical missed count so add to that list
            critical_devices.append(device)
        elif(device['missed_count'] >= warning_missed):
            #hit the warning missed count, so add to that list
            warning_devices.append(device)

    #return the result lists in a tuple
    return (warning_devices, critical_devices, error_devices)
            

def get_data_error_plugins(db):
    """function to get data on each plugin with errors, and how many devices
    under that plugin are affected"""

    #make a cursor
    db_cursor = db.cursor(dictionary=True)
    #dictionary to return at the end
    error_plugins = {}

    #error plugin query template
    #gets the plugins that are down, and how many devices under each
    #in two columns 'plugin' and 'num_devices'
    error_plugin_query = (" SELECT {deviceTable}.plugin, "
            " COUNT({deviceTable}.mac_address) AS num_devices "
            " FROM {deviceTable} "
            " INNER JOIN {monitoringTable} ON "
            " {deviceTable}.mac_address = {monitoringTable}.mac_address "
            " AND {monitoringTable}.missed_count >= %(flag_val)s "
            " GROUP BY plugin ; "
            "").format(deviceTable=deviceTableName, 
                    monitoringTable=monitoringTableName)

    #execute the query and give the flag variable to check with
    db_cursor.execute(error_plugin_query, {'flag_val': MISSED_ERROR_FLAG})
    #get the result
    error_plugins = db_cursor.fetchall()

    if TEST_DB:
        #print it out for test
        print("error plugin query result: ")
        print_result(error_plugins)

    #return the dictionary
    return error_plugins

def get_data_string(data):
    """generate a comma separated data list string"""
    out_str = "{ "
    for item in data:
        out_str += str(item) + ", "
    out_str += "}"
    return out_str

#### BEGIN TIME-OUT AND ERROR MONITORING CODE ####

if TEST_DB:
    #execute listing of the monitoring table before others
    mycursor.execute((" SELECT * FROM {monitoringTable} ;").format(
        monitoringTable=monitoringTableName))
    testing_result = mycursor.fetchall()
    testing_count = mycursor.rowcount
    print("before populate: ")
    print_result(testing_result)

#start executing the monitoring database code

#populate
populate_monitoring_db(wallinkDB)
#cleanup
cleanup_monitoring_db(wallinkDB)
#update values and do checks
update_and_check_times(wallinkDB)

#get the lists for the timeouts and errors
warning_timeout_devices, critical_timeout_devices, data_error_devices = get_timeout_devices(wallinkDB)
#get the error plugin info
data_error_plugins = get_data_error_plugins(wallinkDB)

#### END OF TIME-OUT AND ERROR MONITORING CODE ####

#done with the Database
#Close the SQL connection
mycursor.close()
wallinkDB.close()

#get the total counts
warning_timeout_num = len(warning_timeout_devices)
critical_timeout_num = len(critical_timeout_devices)
data_error_device_num = len(data_error_devices)
#get the error plugin count
data_error_plugin_num = len(data_error_plugins)

#strings for output
output_str = ""
voltage_msg = "Voltage: "
timeout_msg = "Timeout: "
data_error_msg = "Data: "
#variable to store our end exit value/Nagios error code
#set to be OK unless anything triggers otherwise
exit_val = Level.OK

#calculate output

#deal with data error plugins
if((data_error_device_num > 0) or (data_error_plugin_num > 0)):
    #set to critical if there are and data errors
    exit_val = Level.CRITICAL
    data_error_msg += "Error Plugin Count " + str(data_error_plugin_num) + " "
    data_error_msg += get_data_string(data_error_plugins) + " "
else:
    data_error_msg += "all OK "

#the voltage results
if(critical_count > 0):
    exit_val = Level.CRITICAL
    voltage_msg += "critical " + str(critical_count) + " " 
    voltage_msg += get_device_list_str(critical_result) + " "
if(warning_count > 0):
    if(exit_val != Level.CRITICAL): 
        #check if not already CRITICAL, if not, set to WARNING
        exit_val = Level.WARNING
    voltage_msg += "warning " + str(warning_count) + " "
    voltage_msg += get_device_list_str(warning_result) + " "
elif(not warning_count and not critical_count):
    voltage_msg += "all OK "
   
#the timeout results
if(critical_timeout_num > 0):
    exit_val = Level.CRITICAL
    timeout_msg += "critical " + str(critical_timeout_num) + " "
    timeout_msg += get_device_list_str(critical_timeout_devices) + " "
if(warning_timeout_num > 0):
    if(exit_val != Level.CRITICAL): 
        #check if not already CRITICAL, if not, set to WARNING
        exit_val = Level.WARNING
    timeout_msg += "warning " + str(warning_timeout_num) + " "
    timeout_msg += get_device_list_str(warning_timeout_devices) + " "
elif(not warning_timeout_num and not critical_timeout_num):
    timeout_msg += "all OK "

#generate the final output string
output_str += "{} - {} {} {}".format(Level.str(exit_val),
        voltage_msg, timeout_msg, data_error_msg) 

#print out the output string and errors
print(output_str)
#exit with the Nagios exit code
sys.exit(exit_val)

