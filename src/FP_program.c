/*
 * fp_program.c
 *
 * Created: 3/31/2022 5:12:29 PM
 *  Author: mmyra
 */ 
#define F_CPU 12000000UL
#include <xc.h>
#include <util/delay.h>
#include "../inc/FP_interface.h"
#include "../inc/UART_interface.h"
#include "../inc/LCD_interface.h"
#include "../inc/keys_interface.h"

//First three Packages
const char Header[]			= {0xEF, 0x1};
const char Address[]		= {0xFF, 0xFF, 0xFF, 0xFF};
const char Command[]		= {0x1, 0x00};

//commands Packages
const char PassVfy[]		= {0x7, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B};
const char f_detect[]		= {0x3, 0x1, 0x0, 0x5};
const char f_imz2ch1[]		= {0x4, 0x2, 0x1, 0x0, 0x8};
const char f_imz2ch2[]		= {0x4, 0x2, 0x2, 0x0, 0x9};
const char f_createModel[]	= {0x3, 0x5, 0x0, 0x9};
char f_storeModel[]			= {0x6, 0x6, 0x1, 0x0, 0x1, 0x0, 0xE};
const char f_search[]		= {0x8, 0x4, 0x1, 0x0, 0x0, 0x0, 0xA3, 0x0, 0xb1};
char f_delete[]				= {0x7, 0xC, 0x0, 0x0, 0x0, 0x1, 0x0, 0x15};
char fp_empty[]				= {0x3,0xd,0x0,0x11};

char sendcmd2fb(unsigned char order)
{
	unsigned char successed = again;
	while (successed == again)
	{
		////////////////////////////////////////////////////////    //
		///////////////        First Stage         /////////////   ///
		/////////// First massage in first row in LCD //////////    //
		////////////////////////////////////////////////////////    //
		lcdclear();													//
		switch (order)											  //////
		{
			case FB_connect:
				lcdprint("Search 4 Module");
				break;
			
			case F_find:
				lcdprint("Place UR Finger");
				break;
			
			case F_im1:
				lcdprint("Processing..");
				break;
			
			case F_im2:
				lcdprint("Processing..");
				break;
			
			case F_cretModl:
				lcdprint("Creating Model");
				break;
			
			case FP_empty:
				lcdprint("IDs Erasing");
				break;
			
			case F_search:
				lcdprint("Searching...");
				break;
			
			case F_store:
				lcdprint("Storing Model");
				break;
			
			case F_delete:
				lcdprint("Delete Finger");
				break;
		}
		
		////////////////////////////////////////////////////////    ////
		///////////////        Second Stage        /////////////  //   //
		///////////   sending Command to the Module   //////////      //
		////////////////////////////////////////////////////////    //
																  ///////
		//First three Packages (Common between any command)
		UART_Array(&Header ,2);
		UART_Array(&Address,4);
		UART_Array(&Command,2);
		
		int arr_size;
		unsigned char ID;
		
		//Commands to fingerprint module
		switch (order)
		{
			case FB_connect:
				arr_size = (sizeof(PassVfy)) / sizeof((PassVfy)[0]);
				UART_Array(&PassVfy,arr_size);
				break;
			
			case F_find:
				arr_size = (sizeof(f_detect)) / sizeof((f_detect)[0]);
				UART_Array(&f_detect,arr_size);
				break;
			
			case F_im1:
				arr_size = (sizeof(f_imz2ch1)) / sizeof((f_imz2ch1)[0]);
				UART_Array(&f_imz2ch1,arr_size);
				break;
			
			case F_im2:
				arr_size = (sizeof(f_imz2ch2)) / sizeof((f_imz2ch2)[0]);
				UART_Array(&f_imz2ch2,arr_size);
				break;
			
			case F_cretModl:
				arr_size = (sizeof(f_createModel)) / sizeof((f_createModel)[0]);
				UART_Array(&f_createModel,arr_size);
				break;
			
			case FP_empty:
				arr_size = (sizeof(fp_empty)) / sizeof((fp_empty)[0]);
				UART_Array(&fp_empty,arr_size);
				break;
			
			case F_search:
				arr_size = (sizeof(f_search)) / sizeof((f_search)[0]);
				UART_Array(&f_search,arr_size);
				break;
			
			case F_store:
				ID = GetID();
				f_storeModel[4] = ID;
				f_storeModel[6] = (0xE + ID);
				arr_size = (sizeof(f_storeModel)) / sizeof((f_storeModel)[0]);
				UART_Array(&Header ,2);
				UART_Array(&Address,4);
				UART_Array(&Command,2);
				UART_Array(&f_storeModel,arr_size);
				break;
			
			case F_delete:
				ID = GetID();
				f_delete[3] = ID;
				f_delete[7] = (0x15 + ID);
				arr_size = (sizeof(f_delete)) / sizeof((f_delete)[0]);
				UART_Array(&Header ,2);
				UART_Array(&Address,4);
				UART_Array(&Command,2);
				UART_Array(&f_delete,arr_size);
				break;
		}
		_delay_ms(1000);
		
		////////////////////////////////////////////////////////     /////
		///////////////        third  Stage        /////////////   //    //
		/////////   Getting feedback from the module  //////////         //
		////  In case of failure: Print on LCD the reason  /////      ///
		////////////////////////////////////////////////////////		//
																  //    //
		//Feedback (Acknowledge) from fingerprint module			////
		if(cont>1)
		{
			if( rcvData[6] == 0x07 && (rcvData[8] == 0x03 || rcvData[8] == 0x07) )
			{
				lcdsecondrow();
				
				if(rcvData[9] == 0)
				successed = success;
				else
				{
					successed = fail;
					if(rcvData[9] == 0x01)
					lcdprint("Rcv packet error");
					else if(rcvData[9] == 0x04)
					lcdprint("fail.  Try again");
					else if(rcvData[9] == 0x05)
					lcdprint("Fail Clean & Try");
					else if(rcvData[9] == 0x06)
					lcdprint("FailedGenerating");
					else if(rcvData[9] == 0x07)
					lcdprint("SmallFinger Area");
					else if(rcvData[9] == 0x09)
					lcdprint("ID not Found");
					else if(rcvData[9] == 0x0b)
					lcdprint("ID overload");
					else if(rcvData[9] == 0x18)
					lcdprint("FlashWritingErr");
					else if(rcvData[9] == 0x0a)
					lcdprint("Failed modeling");
					else if(rcvData[9] == 0x13)
					lcdprint("Password incorrect");
					else if(rcvData[9] == 0x21)
					lcdprint("must verify pass");
					else
					{
						if(rcvData[9] == 0x02)
						lcdprint("CantDetectFinger");
						else if(rcvData[9] == 0x03)
						lcdprint("Fail To Collect");
						else
						lcdprint("NOT DEFINED ERROR");
						successed = again;
					}
				}
			}
			else
			{
				lcdclear();
				lcdprint("FP module");
				lcdsecondrow();
				lcdprint("connection Error");
			}
		}
		else
		{
			lcdclear();
			lcdprint("FP not Found or");
			lcdsecondrow();
			lcdprint("Not Responding");
			_delay_ms(1000);
		}
		
		////////////////////////////////////////////////////////     /////
		///////////////        Fourth Stage        /////////////    //  //
		/////////  Second massage in Second row in LCD  ////////   //   //
		/////  In case of success: Print on LCD FeedBack  //////  //    //
		//////////////////////////////////////////////////////// ///////////
		if(successed == success)										//
		{
			unsigned char String[20];
			
			switch (order)
			{
				case FB_connect:
					lcdprint("Module Found");
					break;
				
				case F_find:
					lcdprint("Finger Captured");
					break;
				
				case F_im1:
					lcdprint("Done");
					break;
				
				case F_im2:
					lcdprint("Done");
					break;
				
				case F_cretModl:
					lcdprint("Model Created");
					break;
				
				case FP_empty:
					lcdprint("Memory Erased");
					break;
				
				case F_search:
					lcdprint("Found in ID: ");
					sprintf(String," %d  ",rcvData[11]);
					lcdprint(&String);
					break;
				
				case F_store:
					lcdprint("Saved Successful");
					_delay_ms(700);
					break;
				
				case F_delete:
					sprintf(String,"ID: %d Deleted",ID);
					lcdprint(&String);
					_delay_ms(700);
					break;
			}
		}
		clearArray(&rcvData);
		cont = 0;
		_delay_ms(700);
	}
	return successed;
}

char GetID()
{
	lcdclear();
	unsigned char ids = 1;
	char ok = 0;
	lcdprint("Enter ID:");
	while(ok == 0)
	{
		if(keys_Feedback() == keys_up)
		ids++;
		else if(keys_Feedback() == keys_down)
		ids--;
		else if(keys_Feedback() == keys_ok)
		ok = 1;
		if(ids >= 128)
		ids = 1;
		unsigned char String[20];
		lcdgotoxy(1,10);
		sprintf(String," %d  ",ids);
		lcdprint(&String);
		_delay_ms(200);
	}
	return ids;
}

void enroll()
{
	loop:
	if(sendcmd2fb(F_find)) goto loop;
	if(sendcmd2fb(F_im1)) goto loop;
	if(sendcmd2fb(F_find)) goto loop;
	if(sendcmd2fb(F_im2)) goto loop;
	if(sendcmd2fb(F_cretModl)) goto loop;
	if(sendcmd2fb(F_store)) goto loop;
}

void Search()
{
	loop:
	if(sendcmd2fb(F_find)) goto loop;
	if(sendcmd2fb(F_im1)) goto loop;
	if(sendcmd2fb(F_search)) goto loop;
}
