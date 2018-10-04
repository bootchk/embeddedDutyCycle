
#include "blinkPeriod.h"




/*
 * !!! Not static, so they appear in map.
 * Also, if static, might exist multiple instances, whenever this file is included without #pragma once
 */
namespace {

/*
 * Blink period must persist through low power sleep
 */
#pragma PERSISTENT
int counter = 0;

// State variable, evening and morning blink subperiods
#pragma PERSISTENT
bool _isEvening = false;

#pragma PERSISTENT
bool _isActive = false;

}



void BlinkPeriod::initForEveningBlinking()
{
    counter = Parameters::BlinksEvening;
    _isEvening = true;
    _isActive = true;
}


void BlinkPeriod::initForMorningBlinking()
{

    counter = Parameters::BlinksMorning;
    _isEvening = false;
    _isActive = true;
}


bool BlinkPeriod::isActive()
{
    return _isActive;
}


/*
 * Side effect: deactivate when over
 */
bool BlinkPeriod::isOver()
{
    bool result;
    result = (counter <= 0);
    // active if not over
    _isActive = not result;
    return result;
}


bool BlinkPeriod::isEvening()
{
    return _isEvening;
}


void BlinkPeriod::advance()
{
    counter--;
}


