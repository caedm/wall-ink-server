package main

import (
    "encoding/json"
    "fmt"
    "io/ioutil"
    "log"
    "net/http"
    "os"
    "time"
    "flag"

    "golang.org/x/net/context"
    "golang.org/x/oauth2"
    "golang.org/x/oauth2/google"
    "google.golang.org/api/calendar/v3"
)

// Retrieve a token, saves the token, then returns the generated client.
func getClient(config *oauth2.Config) *http.Client {
    tokFile := "token.json"
    tok, err := tokenFromFile(tokFile)
    if err != nil {
        tok = getTokenFromWeb(config)
        saveToken(tokFile, tok)
    }
    return config.Client(context.Background(), tok)
}

// Request a token from the web, then returns the retrieved token.
func getTokenFromWeb(config *oauth2.Config) *oauth2.Token {
    authURL := config.AuthCodeURL("state-token", oauth2.AccessTypeOffline)
    fmt.Printf("Go to the following link in your browser"+
    ": \n%v\n", authURL)
    fmt.Printf("\nPaste the authorization code below:\n")

    var authCode string
    if _, err := fmt.Scan(&authCode); err != nil {
        log.Fatalf("Unable to read authorization code: %v", err)
    }

    tok, err := config.Exchange(oauth2.NoContext, authCode)
    if err != nil {
        log.Fatalf("Unable to retrieve token from web: %v", err)
    }
    return tok
}

// Retrieves a token from a local file.
func tokenFromFile(file string) (*oauth2.Token, error) {
    f, err := os.Open(file)
    defer f.Close()
    if err != nil {
        return nil, err
    }
    tok := &oauth2.Token{}
    err = json.NewDecoder(f).Decode(tok)
    return tok, err
}

// Saves a token to a file path.
func saveToken(path string, token *oauth2.Token) {
    fmt.Printf("Saving credential file to: %s\n", path)
    f, err := os.OpenFile(path, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0600)
    defer f.Close()
    if err != nil {
        log.Fatalf("Unable to cache oauth token: %v", err)
    }
    json.NewEncoder(f).Encode(token)
}

func main() {

    secPtr := flag.String("sec","client_secret.json","Path to Google Client Secret File. \n\n        To enable the API and create a client secret file: \n        Go to  https://developers.google.com/calendar/quickstart/go \n        Follow instructions under \"Turn on the Google Calendar API.\"\n        A token.json file will be saved in the working directory if one does not exist.\n\n      Example: gcal -sec=secret_folder/my_client_secret_file.json\n")

    calPtr := flag.String("cal","primary","Calendar to get all of today's events from. \n\n        Default calendar for a user is \"primary\" or the gmail email address for the user.\n        Shared primary calendars for other users will be the gmail email address for the other user.\n\n        For other calendars, use the CalendarID in the format of <longstringofrandomcharacters>@group.calendar.google.com.\n        Find the CalendarID under settings in the Google Calendar website.\n\n      Example1: gcal -cal=foo@gmail.com\n      Example2: gcal -cal=umsb0ekhivs1a2ubtq6vlqvckj@group.calendar.google.com\n") 
    flag.Parse()	
    //fmt.Println("API key:", *keyPtr)
    //fmt.Println("Calendar:", *calPtr)

    b, err := ioutil.ReadFile(*secPtr)
    if err != nil {
        log.Fatalf("Unable to read client secret file: %v", err)
    }

    // If modifying these scopes, delete your previously saved client_secret.json.
    config, err := google.ConfigFromJSON(b, calendar.CalendarReadonlyScope)
    if err != nil {
        log.Fatalf("Unable to parse client secret file to config: %v", err)
    }

    srv, err := calendar.New(getClient(config))
    if err != nil {
        log.Fatalf("Unable to retrieve Calendar client: %v", err)
    }
    date_today := time.Now()
    today_date := date_today.Format("2006-01-02")
    begin_today := today_date + "T00:00:00-07:00"	
    end_today := today_date + "T23:59:59-07:00"

    events, err := srv.Events.List(*calPtr).ShowDeleted(false).
    SingleEvents(true).TimeMin(begin_today).TimeMax(end_today).OrderBy("startTime").Do()
    if err != nil {
        log.Fatalf("Unable to retrieve next ten of the user's events: %v", err)
    }

    fmt.Println(events.Summary)
    //fmt.Println(date_today.Format("2006-01-02"))
    //fmt.Println(date_today.Format("15:04"))

    for _, item := range events.Items {
        start_date := item.Start.DateTime
        if start_date == "" {
            start_date = item.Start.Date
        }
        fmt.Printf("%v \n", item.Summary)
        s_date, err := time.Parse("2006-01-02T15:04:05Z07:00", start_date)
        if err == nil {
            fmt.Printf("%v \n", s_date.Format("2006-01-02 15:04:05")) 
        }
        end_date := item.End.DateTime

        if end_date == "" {
            end_date = item.End.Date
        }
        e_date, err := time.Parse("2006-01-02T15:04:05Z07:00", end_date)
        if err == nil {
            fmt.Printf("%v \n", e_date.Format("2006-01-02 15:04:05")) 
        }

    }
}
