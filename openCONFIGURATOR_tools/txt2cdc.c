/**
********************************************************************************
\file   txt2cdc.c

\brief  Convert a textual CDC file (.txt) into its binary version (.cdc). 

The input file contains hex values in character type which are converted to
binary format. The output is stored in a binary file.

\copyright (c) 2014, Kalycito Infotech Private Limited
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holders nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "txt2cdc.h"

//------------------------------------------------------------------------------
/**
\brief  This function converts the ASCII value of character to hex value

This function gets the parameter of char type at size of 1 byte and converts
the ASCII to hex value of 0-9 and A-F.

\param[in]  str_p           Character value of 8 bit

\return str_p           Hex value of 8 bit
\retval Returns hex value of 0-9 and A-F
*/
//------------------------------------------------------------------------------
INT8 asciiToHex(INT8 str_p)
{
    DEBUG_INFO("\n Entered the asciiToHex Function\n");

    if (str_p >= 65 && str_p <= 70)
    {
        str_p = str_p - 55;
    }
    else if (str_p >=48 && str_p <= 57)
    {
        str_p = str_p - 48;
    }
    else
    {
        //not needed
    }

    return str_p;
}

//------------------------------------------------------------------------------
/**
\brief  This function reverses the source element and stores in destination

This function gets the two pointer to the array of elements and size of the
array element. Checks both the pointer has element in it. if not, returns general
error. The One array of element is reversed with size and is stored in memory of
another pointer to the array of element. After the completion of reversal, it
returns 0.

\param[out]  pData1_p        Pointer to the array gets the reversed value from source
                        memory
\param[in]  pData2_p        Pointer to the array of elements reverses the data and
                        delivers to destination
\param[in]  size_p          Size in bytes of buff to get reversed

\return INT32
\retval -1              Error if there is no data in any pointer variable
        0               Confirms the data reversal
*/
//------------------------------------------------------------------------------
INT32 reverseData(UINT8* pData1_p, UINT8* pData2_p, UINT32 size_p)
{
    UINT32 count = 0;

    DEBUG_INFO("\n Entered the reverseData Function\n");

    if (pData1_p == NULL || pData2_p == NULL)
    {
        return kError;
    }

    for (count = 0; count < size_p; count++)
    {
        *(pData1_p + count) = *(pData2_p + size_p - 1 - count);
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
\brief  This function converts the text format to binary format.

This main function open the two file name from command line argument.
    arg1:   Input text file (.txt)
    arg2:   Output binary file (.cdc)
Opens the input file in read only mode and output file in write binary mode. The
input file pointer is accessed to count the number of values(0-9, A-Z) present
in it. Two buffer is allocated with size of count and default buffer size.
The elements in input file converts ASCII to binary and stores in buffer1.
The byte packing combines two binary 4bit to 1 byte of data and is stored in
buffer2. The elements in Buffer2 is separated regarding the Index, SubIndex,
Data size and Data of input file and is reversed individually and delivered to
buffer1. The elements in buffer1 writes to the output file name with file
pointer. General errors like invalid no. of command line arguments, failure in
file opening and error in buffer memory allocation.

\param[in]  argc            Number of input given in command line argument
\param[in]  argv[]          Array of characters from command line argument

\return tResult         Enumerated type contains return values
\retval -1              General error in this program
        0               Confirms the text file is converted to Binary file
        1               Error if text length does not match
*/
//------------------------------------------------------------------------------
INT32 main(INT32 argc, char* argv[])
{
    FILE*   pInputTxt           = NULL;
    FILE*   pOutputCdc          = NULL;

    INT32   char_attr           = 0;        ///< Single char
    UINT32  count1              = 0;        ///< Counter 1
    UINT32  count2              = 0;        ///< Counter 2
    UINT32  length              = 0;        ///< Number of entries
    UINT32  lengthMNDat         = 0;        ///< Number of lines in configure data
    UINT32  lengthDat           = 0;        ///< Data size in input file

    UINT8*  pBuff1              = NULL;     ///< Character buffer
    UINT8*  pBuff2              = NULL;     ///< Character buffer

    #if defined(_WIN32) && defined(_MSC_VER)
        errno_t     ret;
    #else
        UINT32  ret             = 0;        ///< Return error value
    #endif

    DEBUG_INFO("\n Entered the Main Function\n");

    if (argc <= 2 || argc > 3)
    {
        printf("\n Invalid Arguments \n");
        return kError;
    }

    #if defined(_WIN32) && defined(_MSC_VER)
        ret = fopen_s(&pInputTxt, argv[1], "r");
        if (ret != 0)
        {
        printf("\n Error opening input file \n");
            return kError;
        }
    #else
        pInputTxt = fopen(argv[1], "r");
        if (pInputTxt == NULL)
        {
            printf("\n Error opening input file \n");
            return kError;
        }
    #endif

    #if defined(_WIN32) && defined(_MSC_VER)
        ret = fopen_s(&pOutputCdc, argv[2], "wb");
        if (ret != 0)
        {
            printf("\n Error opening cdc file \n");
            return kError;
        }
    #else
        pOutputCdc = fopen(argv[2], "wb");
        if (pOutputCdc == NULL)
        {
            printf("\n Error opening cdc file \n");
            fclose(pInputTxt);
            return kError;
        }
    #endif

    //Get the number of entries to allocate memory

    DEBUG_INFO("Entering the entry count Loop\n");

    while (char_attr != EOF)
    {
        char_attr = fgetc(pInputTxt);
        if (char_attr == '/')
        {
            while (fgetc(pInputTxt) != '\n');
        }
        else if (char_attr == 0xd)
        {
            //printf("Check");
        }
        else if (!(char_attr == '\t' || char_attr == '\n' || char_attr == ' '))
        {
            //Convert to Upper case
            char_attr = toupper(char_attr);
            if (char_attr >= 0x30 && char_attr <= 0x39 || char_attr >= 0x41 && char_attr <= 0x5A)
            {
                count1++;
            }
        }
        else
        {
            //Not needed
        }
    }

    DEBUG_INFO("Exited the entry count loop\n Number of Entries = %d\n", count1);

    //Store the number of entries(characters in the file that of interest to us)
    length = count1;
    pBuff1 = (UINT8*)malloc(length + kBufSize_g);

    if (pBuff1 == NULL)
    {
        printf("\n Error! Memory allocation failed for Odd\n");
        fclose(pInputTxt);
        fclose(pOutputCdc);
        return kError;
    }

    pBuff2 = (UINT8*)malloc(length + kBufSize_g);

    if (pBuff2 == NULL)
    {
        printf("\n Error! Memory allocation failed for Odd\n");
        free(pBuff1);
        fclose(pInputTxt);
        fclose(pOutputCdc);
        return kError;
    }

    //Move the file pointer to the beginning of the file
    rewind(pInputTxt);

    count1 = 0;
    char_attr = 0;

    DEBUG_INFO("\n Entering the process Loop\n");

    while (char_attr != EOF)
    {
        char_attr = fgetc(pInputTxt);
        if (char_attr == '/')
        {
            while (fgetc(pInputTxt) != '\n');
        }
        else if (char_attr == 0xd)
        {
            DEBUG_INFO("Check");
        }
        else if (!(char_attr == '\t' || char_attr == '\n' || char_attr == ' '))
        {
            // Convert to Upper case
            char_attr = toupper(char_attr);
            //Convert Char to Binary Value
            char_attr = asciiToHex(char_attr);
            if (char_attr >= 0 && char_attr <= 0xF)
            {
                pBuff1[count1] = char_attr;
                count1++;
            }
        }
        else
        {
            //Not Needed
        }
    }

    DEBUG_INFO("\n Exited the process loop\n");

    //Store the number of entries(characters in the file that of interest to us)
    length = count1;

    //For testing purpose.
    for ( count1 = 0; count1 < (length - 1); count1++)
    {
        DEBUG_INFO("%x, ", pBuff1[count1]);
    }

    //For Byte Packing
    for (count1 = 0, count2 = 0; count1 < (length - 1); count1++, count2++)
    {
        count1 = count2 * 2;
        if ((length - 1) <= count1)
        {
            printf("\n Error, Incorrect formatting of the text file! \n");
            return kTextFileFormattingError_g;
        }
        else
        {
            pBuff2[count2] = (UINT8)((pBuff1[count1] << 4) | pBuff1[count1 + 1]);
        }
        DEBUG_INFO("%.2x, ", pBuff2[count2]);
    }

    //Store the number of entries(characters in the file that of interest to us)
    length = count2;

    DEBUG_INFO("\n Size : %x\n", length);

    reverseData(&pBuff1[kEntryMn_g], &pBuff2[kEntryMn_g], sizeof(UINT32));

    DEBUG_INFO("\n Number of Entries: %x\n", *(UINT32*)&pBuff1[0]);

    count1 = kFirstIndMnObd_g; //Skip the number of elements field

    //For Byte Ordering
    DEBUG_INFO("\n Entering the byte ordering loop\n");

    while (count1 < (length - 1))
    {
        //Index
        reverseData(&pBuff1[count1 + kIndex_g], &pBuff2[count1 + kIndex_g], sizeof(UINT16));
        //SubIndex
        pBuff1[count1 + kSubIndex_g] = pBuff2[count1 + kSubIndex_g];
        //DataSize
        reverseData(&pBuff1[count1 + kDataSize_g], &pBuff2[count1 + kDataSize_g], sizeof(UINT32));
        lengthMNDat = *((UINT32*)&pBuff1[count1 + kDataSize_g]);
        //Data should also be reversed correctly and also if its 1F22's data it needs special treatment
        if (*(UINT16*)&pBuff1[count1] == 0x1F22)
        {
            DEBUG_INFO("\n Size : %x\n", lengthMNDat);

            //Just after the index, subindex and size we will have the number of entries
            reverseData(&pBuff1[count1 + kEntryCn_g], &pBuff2[count1 + kEntryCn_g], sizeof(UINT32));

            DEBUG_INFO("\n Number of Entries: %x\n", *(UINT32*)&pBuff1[count1 + kEntryCn_g]);

            count2 = count1 + kFirstInd1F22_g; //first index within the 1F22 object
            while (count2 < (count1 + kData_g + lengthMNDat - 1))
            {
                //Index
                reverseData(&pBuff1[count2 + kIndex_g], &pBuff2[count2 + kIndex_g], sizeof(UINT16));
                //SubIndex
                pBuff1[count2 + kSubIndex_g] = pBuff2[count2 + kSubIndex_g];
                //DataSize
                reverseData(&pBuff1[count2 + kDataSize_g], &pBuff2[count2 + kDataSize_g], sizeof(UINT32));
                lengthDat = *(UINT32*)&pBuff1[count2 + kDataSize_g];
                //Data should also be reversed correctly
                reverseData(&pBuff1[count2 + kData_g], &pBuff2[count2 + kData_g], lengthDat);

                DEBUG_INFO("\tIndex: %x SubIndex: %x Size %x\n", *(UINT16*)&pBuff1[count2 + kIndex_g], *(UINT8*)&pBuff1[count2 + kIndex_g], *(UINT32*)&pBuff1[count2 + kDataSize_g]);

                count2 = count2 + kData_g + lengthDat;
            }
        }
        else
        {
            DEBUG_INFO("\n Index: %x SubIndex: %x Size %x\n", *(UINT16*)&pBuff1[count1 + kIndex_g], *(UINT8*)&pBuff1[count1 + kSubIndex_g], lengthMNDat);

            //Get the length of the data to be reversed
            reverseData(&pBuff1[count1 + kData_g], &pBuff2[count1 + kData_g], lengthMNDat);
        }
        DEBUG_INFO("\n Index: %x SubIndex: %x Size %x\n", *(UINT16*)&pBuff1[count1 + kIndex_g], *(UINT8*)&pBuff1[count1 + kSubIndex_g], lengthMNDat);

        count1 = count1 + kData_g + lengthMNDat ;
    }
    DEBUG_INFO("\n Exited the byte ordering loop\n");

    // Write to Binary file
    ret = fwrite(pBuff1, 1, length, pOutputCdc);

    DEBUG_INFO("\n Finished writing the data\n");

    if ( ret != length )
    {
        printf("\n Error! fwrite failed\n");
    }

    //Close the file
    fclose(pInputTxt);
    fclose(pOutputCdc);
    //Free the memory allocate to the variables
    free(pBuff1);
    free(pBuff2);

    return kTxt2CdcSuccess_g;
}
