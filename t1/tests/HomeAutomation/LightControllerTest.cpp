//- ------------------------------------------------------------------
//-    Copyright (c) James W. Grenning -- All Rights Reserved         
//-    For use by owners of Test-Driven Development for Embedded C,   
//-    and attendees of Renaissance Software Consulting, Co. training 
//-    classes.                                                       
//-                                                                   
//-    Available at http://pragprog.com/titles/jgade/                 
//-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3               
//-                                                                   
//-    Authorized users may use this source code in your own          
//-    projects, however the source code may not be used to           
//-    create training material, courses, books, articles, and        
//-    the like. We make no guarantees that this source code is       
//-    fit for any purpose.                                           
//-                                                                   
//-    www.renaissancesoftware.net james@renaissancesoftware.net      
//- ------------------------------------------------------------------


extern "C"
{
#include "LightController.h"
#include "LightDriverSpy.h"
}

#include "CppUTest/TestHarness.h"

//START: turnOn
TEST_GROUP(LightController)
{
    LightDriver spy;

    void setup()
    {
      LightController_Create();
      LightDriverSpy_AddSpiesToController();
    }

    void teardown()
    {
       LightController_Destroy();
    }
};

TEST(LightController, TurnOn)
{
	LightController_TurnOn(7);
	LONGS_EQUAL(LIGHT_ON, LightDriverSpy_GetState(7));
}
//END: turnOn

TEST(LightController, TurnOff)
{
	LightController_TurnOff(1);
	LONGS_EQUAL(LIGHT_OFF, LightDriverSpy_GetState(1));
}

/* START: AddingDriverDestroysPrevious */
TEST(LightController, AddingDriverDestroysPreviousAndDoesNotLeak)
{
    LightDriver spy = LightDriverSpy_Create(1);
    LightController_Add(1, spy);
    LightController_Destroy();
}
/* END: AddingDriverDestroysPrevious */

TEST(LightController, AllDriversDestroyed)
{
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		LightDriver spy = LightDriverSpy_Create(i);
		LONGS_EQUAL(TRUE, LightController_Add(i, spy));
	}
}

TEST(LightController, ValidIdLowerRange)
{
	LightDriver spy = LightDriverSpy_Create(0);
	LONGS_EQUAL(TRUE, LightController_Add(0, spy));
}

TEST(LightController, ValidIdUpperRange)
{
	LightDriver spy = LightDriverSpy_Create(MAX_LIGHTS);
	LONGS_EQUAL(TRUE, LightController_Add(MAX_LIGHTS-1, spy));
}

TEST(LightController, InValidIdBeyondUpperRange)
{
	LightDriver spy = LightDriverSpy_Create(MAX_LIGHTS);
	LONGS_EQUAL(FALSE, LightController_Add(MAX_LIGHTS, spy));
	free(spy);
}

TEST(LightController, NonAddedLightDoesNothing)
{
    LightController_Add(1, NULL);
    LightController_TurnOn(1);
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
    LightController_TurnOff(1);
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightDriverSpy_GetState(1));
}



