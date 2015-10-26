/*
*------------------------------------------------------------------------------
* linearkeypad.c
*
* Lead per key linear keypad module. 
* Port B is used as the keypad port.
* Maximum no of keys supported - 8.
* Debounce Count = 4 .
* 
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* INCLUDES
*------------------------------------------------------------------------------
*/

#include "linearkeypad.h"



/*
*------------------------------------------------------------------------------
* Structure declarations
*------------------------------------------------------------------------------
*/


typedef struct _Key
{
	KEY_STATE state;
	UINT8 debounceCount;
}Key;




/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/




UINT8 portValue = 0xFF;

Key keys[MAX_KEYS]={0};
UINT8 keyPin = 0xFF;


/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/


static void updateKey(Key *key , UINT8 index);

PIN_STATE readKey(void);

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
*void LinearKeyPad_init(  )
*
*Function to initialize the linear keypad. 
* 1.Sets the direction of the keypad port i.e PORTB to input.
* 2. Initializes the state and debounceCount of the Key objects
* Input: keys -- Array of Key objects 
*		 noKeys -- number of Key objects
*		 Keys are assumed to start from index 0.
*
* output: the direction of keypad port is initialized. The Key objects
* 			are initialized.
*
* return value: none.
* 
*------------------------------------------------------------------------------
*/

void LinearKeyPad_init(UINT8 key )
{
	LinearKeyPad_reset();
	keyPin = key;
}


/*
*------------------------------------------------------------------------------
*void LinearKeyPad_updateKey( Key *Keys , UINT8 keyIndex )
*
*Function to update the state of a single key
*
* Input: Keys -- Array of Key objects 
*		 keyIndex -- the index of the key whose state has to be updated
*		 Keys are assumed to start from index 0.
*
* output: the state of the Key object is updated according to the
* 		  pin state of the key pad  port.
*
* return value: none.
* 
*------------------------------------------------------------------------------
*/
/*
void LinearKeyPad_updateKey( Key *key , UINT8 keyIndex )
{
	readKeyPadPort();
	updateKey( key , keyIndex);

}	
*/


/*
*------------------------------------------------------------------------------
*void LinearKeyPad_scan( Key *keys , UINT8 fromIndex , UINT8 toIndex )
*
*Function to scan a subset of the keys in the linear keypad.
*This scans the keys starting from "fromIndex" to "toIndex"
* both inclusive.
*
*
* Input: keys -- Array of Key objects 
*		 fromIndex -- starting index of the key.
*		 toIndex -- ending index of the key.
*
* output: the state of each of the Key objects is updated according to the
* 		  pin state of the key pad  port.
*
* return value: none.
* 
*------------------------------------------------------------------------------
*/
/*
void LinearKeyPad_scan_from_to( Key *keys , UINT8 fromIndex , UINT8 toIndex )
{
	UINT8 i;
	for( i = fromIndex ; i <= toIndex ; i++)
	{
		readKeyPadPort();
		updateKey( &keys[i] , i );
	}
}

*/
/*
*------------------------------------------------------------------------------
*void LinearKeyPad_scan(  )
*
*Function to scan the linear keypad.
*
* Input: keys -- Array of Key objects 
*		 noKeys -- number of Key objects
*
* output: the state of each of the Key objects is updated according to the
* 		  pin state of the key pad  port.
*
* return value: none.
* 
*------------------------------------------------------------------------------
*/


void LinearKeyPad_scan(  )
{
	UINT8 i = 0;


#ifdef __SIMULATION__
portValue = getSimulatedInput();
#else

if( keyPin == 0xFF )
	return;

	//readKeyPadPort();
	readKey();
#endif

	for( i = 0; i < MAX_KEYS ; i++)
	{
		updateKey( &keys[i] , i );
	}



}
		
	

/*
*------------------------------------------------------------------------------
*UINT8 LinearKeyPad_getKeyState( UINT8 index )
*
*Function which returns the state of the key 
*
* Input: index - index of the key whose state is requested
*
*
* output: NONE
*
* return value: state of the key -- KEY_PRESSED/KEY_RELEASED
* 
*------------------------------------------------------------------------------
*/


UINT8 LinearKeyPad_getKeyState( UINT8 index )
{
	return keys[index].state;
	
}


/*
*------------------------------------------------------------------------------
*void LinearKeyPad_reset( )
*
*Function to reset the linear keypad.
*
* Input: none
*
* output: the state and debounce count of the keys are reset.
*
* return value: none.
* 
*------------------------------------------------------------------------------
*/


void LinearKeyPad_reset(  )
{
	UINT8 i;
	for( i = 0 ; i < MAX_KEYS ; i++)
	{
		keys[i].state = KEY_RELEASED;
		keys[i].debounceCount = 0;
	}

#ifdef __SIMULATION__
resetSimulatedInput();
#endif


}




/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/
	

/*
*------------------------------------------------------------------------------
*void readKeyPadPort(  )
*
*function to read the key pad port.
*
* Input: none
*
* output: the static variable portValue is updated with the keypad port value
*
* return value: none.
* 
*------------------------------------------------------------------------------
*/
/*
static void readKeyPadPort()
{
	portValue = KEYPAD_PORT;
}
*/

/*
*------------------------------------------------------------------------------
*void readKey(  )
*
*function to read the pin value associated with the key.
*
* Input: none
*
* output: the static variable portValue is updated with the keypad port value
*
* return value: none.
* 
*------------------------------------------------------------------------------
*/


PIN_STATE readKey()
{
	portValue = 0xFF;
	switch(keyPin)
	{
		case KEY0:
		return (portValue&=(KEY_PIN_0));

		case KEY1:
		return (portValue&=(KEY_PIN_1<<1));

		case KEY2:
		return (portValue&=(KEY_PIN_2<<2));

		case KEY3:
		return (portValue&=(KEY_PIN_3<<3));

		case KEY4:
		return (portValue&=(KEY_PIN_4<<4));

		case KEY5:
		return (portValue&=(KEY_PIN_5<<5));

		case KEY6:
		return (portValue&=(KEY_PIN_6<<6));

		case KEY7:
		return (portValue&=(KEY_PIN_7<<7));

		default:
		return portValue;
	}	
}


/*
*------------------------------------------------------------------------------
*void updateKey(  )
*
*function to read the key pad port.
*
* Input: none
*
* output: the static variable portValue is updated with the keypad port value
*
* return value: none.
* 
*------------------------------------------------------------------------------
*/

static void updateKey(Key *key , UINT8 index)
{
		switch( key->state)
		{
			case KEY_RELEASED:
				if( ( portValue & (1 << index))  == PIN_CLOSED )
				{
					(key->debounceCount)++;
					if((key->debounceCount) >= DEBOUNCE_COUNT ) 
					{
						key->state = KEY_PRESSED;
						key->debounceCount = 0;
					}
				}
				else 
				{
						key->debounceCount = 0;
				}
				
					
			break;
			
			case KEY_PRESSED:
				if( ( portValue & (1 << index ))   )
				{
					(key->debounceCount)++;
					if((key->debounceCount) >= DEBOUNCE_COUNT ) 
					{
						key->state = KEY_RELEASED;
						key->debounceCount = 0;
					}
				}

				else 
				{
						key->debounceCount = 0;
				}
			break;

			default:
			break;
		}
}