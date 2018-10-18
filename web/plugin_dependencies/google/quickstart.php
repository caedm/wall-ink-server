<?php
require __DIR__ . '/vendor/autoload.php';

/**
 * Returns an authorized API client.
 * @return Google_Client the authorized client object
 */
function getClient()
{
    $client = new Google_Client();
    $client->setApplicationName('Google Calendar API PHP Quickstart');
    $client->setScopes(Google_Service_Calendar::CALENDAR_READONLY);
    $client->setAuthConfig("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/google/client_secret.json");
    $client->setAccessType('offline');

    // Load previously authorized credentials from a file.
    $credentialsPath = "$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/google/token.json";
    if (file_exists($credentialsPath)) {
        $accessToken = json_decode(file_get_contents($credentialsPath), true);
    } else {
        // Request authorization from the user.
        $authUrl = $client->createAuthUrl();
        printf("Open the following link in your browser:\n%s\n", $authUrl);
        print 'Enter verification code: ';
        $authCode = trim(fgets(STDIN));

        // Exchange authorization code for an access token.
        $accessToken = $client->fetchAccessTokenWithAuthCode($authCode);

        // Store the credentials to disk.
        if (!file_exists(dirname($credentialsPath))) {
            mkdir(dirname($credentialsPath), 0700, true);
        }
        file_put_contents($credentialsPath, json_encode($accessToken));
        printf("Credentials saved to %s\n", $credentialsPath);
    }
    $client->setAccessToken($accessToken);

    // Refresh the token if it's expired.
    if ($client->isAccessTokenExpired()) {
        $client->fetchAccessTokenWithRefreshToken($client->getRefreshToken());
        file_put_contents($credentialsPath, json_encode($client->getAccessToken()));
    }
    return $client;
}

/**
 * Expands the home directory alias '~' to the full path.
 * @param string $path the path to expand.
 * @return string the expanded path.
 */
function expandHomeDirectory($path)
{
    $homeDirectory = getenv('HOME');
    if (empty($homeDirectory)) {
        $homeDirectory = getenv('HOMEDRIVE') . getenv('HOMEPATH');
    }
    return str_replace('~', realpath($homeDirectory), $path);
}

// Set Timezone as set by CAEDM
date_default_timezone_set('America/Denver');


// Get the API client and construct the service object.
$client = getClient();
$service = new Google_Service_Calendar($client);

$calendarList = $service->calendarList->listCalendarList();

while(true) {
  $pageToken = $calendarList->getNextPageToken();
  if ($pageToken) {
    $optParams = array('pageToken' => $pageToken);
    $calendarList = $service->calendarList->listCalendarList($optParams);
  } else {
    break;
  }
}


function google_getSchedule($your_google_calendar) {
    
    date_default_timezone_set('America/Denver');
    // Get the API client and construct the service object.
    $client = getClient();
    $service = new Google_Service_Calendar($client);


    $your_google_calendar = 'primary';
    $calendar= $service->calendars->get($your_google_calendar);
    $schedule=$calendar->getSummary();

    $date_today = date('Y-m-d'); 
    $begin_today = $date_today . 'T00:00:00-07:00'; //make sure to modify for local time zone
    $end_today = $date_today . 'T23:59:59-07:00';  //make sure to modify for local time zone

    $optParams = array('orderBy' => 'startTime','singleEvents' => 'true','timeMax' => $end_today,'timeMin' => $begin_today,'showDeleted' => 'false');
    $events = $service->events->listEvents($your_google_calendar,$optParams);


    foreach ($events->getItems() as $event) {
        $schedule .= $event->getSummary(); 
        $schedule .= "\n";
        $schedule .= $event->getStart()->dateTime;
        $schedule .= "\n";
        $schedule .= $event->getEnd()->dateTime;
        $schedule .= "\n";
    }
    return $schedule;

}
