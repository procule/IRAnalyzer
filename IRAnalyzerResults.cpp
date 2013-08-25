#include "IRAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "IRAnalyzer.h"
#include "IRAnalyzerSettings.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <bitset>

IRAnalyzerResults::IRAnalyzerResults( IRAnalyzer* analyzer, IRAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

IRAnalyzerResults::~IRAnalyzerResults()
{
}

void IRAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	char b_string[128] = "";

	Frame frame = GetFrame( frame_index );

	if (frame.mData1 == 0x1)
	{
		sprintf( b_string, "Leading burst");
		AddResultString(b_string);
	}
	if (frame.mData1 == 0x2)
	{
		sprintf( b_string, "Space");
		AddResultString(b_string);
	}

	if (frame.mData1 == 0x4)
	{
		sprintf( b_string, "Repeat");
		AddResultString(b_string);
	}

	switch (frame.mData1 & 0xF0)
	{
		case (IRAnalyzerResults::ADDR):
			sprintf(b_string, "ADDR: ");
			break;
		case (IRAnalyzerResults::CMD):
			sprintf(b_string, "CMD: ");
			break;
		case (IRAnalyzerResults::ICMD):
			sprintf(b_string, "ICMD: ");
			break;
		case (IRAnalyzerResults::IADDR):
			sprintf(b_string, "IADDR: ");
			break;

	}

	frame.mData1 &= U64(0xF);

	if (frame.mData1 == 0x8)
	{
		U8 addr = frame.mData2;

		AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 8, b_string+strlen(b_string), 128 );
		AddResultString( b_string );
	}
}

void IRAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void IRAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
	Frame frame = GetFrame( frame_index );
	ClearResultStrings();

	/*char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	AddResultString( number_str );*/
}

void IRAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	ClearResultStrings();
	AddResultString( "not supported" );
}

void IRAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	ClearResultStrings();
	AddResultString( "not supported" );
}
