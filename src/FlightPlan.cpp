#include "FlightPlan.hpp"
#include "main.hpp"

FlightPlan::FlightPlan(){
    state = Waiting;
}

void FlightPlan::update(AltFilter filter){

    float velocity = filter.getVelocity();
    float altitude = filter.getAltitude() - pad_alts[0]; //convert ASL to AGL!

    switch (state)
    {
    case Waiting:
        if(velocity > 100.0){ //launch!
            state = Flight;
            sys.pyro.arm();
            logState();
        }else{
            pad_alt_counter++;
            if(pad_alt_counter >= 50){
                pad_alt_counter = 0;
                pad_alts[0] = pad_alts[1];
                pad_alts[1] = filter.getAltitude();
            }
        }
        break;
    case Flight:
        if(velocity < 0.0){ //apogee!
            state = Falling;
            logState();
        }
        break;
    case Falling:
        if(velocity > -1.0){ //stopped falling
            state = Landed;
            logState();
        }
        break;
    case Landed:
        //do nothing
        break;
    }

    for (uint8_t i = 0; i > sizeof(eventList)/sizeof(FlightEvent); i++)
    {
        FlightEvent e = eventList[i];

        if(state == e.state &&  //state is right
            ((e.velCond == VelNone) || (e.velCond == VelLess && velocity < e.velocity) || (e.velCond == VelMore && velocity > e.velocity)) && //check velocity threshold
            ((e.altCond == AltNone) || (e.altCond == AltLess && altitude < e.altitude) || (e.altCond == AltMore && altitude > e.altitude)))   //check altitude threshold
            {
                StaticJsonDocument<500> event_json;
                event_json["squib"] = (uint8_t) e.squib;
                sys.tasks.logger.logJSON(event_json, "event");
                sys.pyro.fire(e.squib);
            }   
    }

    if(xTaskGetTickCount() - print_timer > 500){
        print_timer = xTaskGetTickCount();
        logState();
    }    
}

void FlightPlan::logState(){
    StaticJsonDocument<500> json;
    json["state"] = (uint8_t) state;
    json["pad_alt"] = pad_alts[0];
    sys.tasks.logger.logJSON(json, "flight_state");
}

//TODO: Strongly Typed Enums, and an implicit conversion to strings for logging
void FlightPlan::dumpConfig(){
    StaticJsonDocument<1000> flightplan_json;
    JsonArray events_json = flightplan_json.createNestedArray("events");
    uint8_t num_events = sizeof(eventList)/sizeof(FlightEvent);
    flightplan_json["count"] = num_events;

    for (uint8_t i = 0; i < num_events; i++)
    {
        JsonObject event_json = events_json.createNestedObject();
        event_json["state"] = (uint8_t) eventList[i].state;

        if(eventList[i].velCond != VelNone){
            event_json["velCond"] = (uint8_t) eventList[i].velCond;
            event_json["velocity"] = eventList[i].velocity;
        }

        if(eventList[i].altCond != AltNone){
            event_json["altCond"] = (uint8_t) eventList[i].altCond;
            event_json["altitude"] = eventList[i].altitude;
        }

        if(eventList[i].action == BlowSquib){
            event_json["squib"] = (uint8_t) eventList[i].squib;
        }
    }
    sys.tasks.logger.logJSON(flightplan_json, "flightplan");
}