
struct TimeStamp{
 int day_of_year;
 int day_of_week;
 int week_number;
 String datetime;
 int ye;
 int mo;
 int da;
 int ho;
 int mi;
 int se;
 void get_time(TimeStamp* ts ,String json){
    int last_year;
    String date_time;

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    (*ts).day_of_year = doc["day_of_year"];
    (*ts).day_of_week = doc["day_of_week"];
    (*ts).day_of_week = doc["week_number"];
    (*ts).datetime = String(doc["datetime"]);

    date_time = (*ts).datetime;
    Serial.println(date_time);

    (*ts).ye = str2int(String(date_time[0]) + String(date_time[1]) + String(date_time[2]) + String(date_time[3]));

    (*ts).mo = str2int(String(date_time[5]) + String(date_time[6]));
    (*ts).da = str2int(String(date_time[8]) + String(date_time[9]));
    (*ts).ho = str2int(String(date_time[11]) + String(date_time[12]));
    (*ts).mi = str2int(String(date_time[14]) + String(date_time[15]));
    (*ts).se = str2int(String(date_time[17]) + String(date_time[18]));
 }

} time_clock;
