#include "IRAnalyzer.h"
#include "IRAnalyzerSettings.h"
#include "IRAnalyzerResults.h"
#include <AnalyzerChannelData.h>

#include <iostream>
#include <bitset>

IRAnalyzer::IRAnalyzer()
:	Analyzer(),  
	mSettings( new IRAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

IRAnalyzer::~IRAnalyzer()
{
	KillThread();
}

void IRAnalyzer::WorkerThread()
{
	U64 tick, ntick, mtick, sample;
	mResults.reset( new IRAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );

	mSampleRateHz = GetSampleRate();

	mSerial = GetAnalyzerChannelData( mSettings->mInputChannel );

	U32 samples_per_bit = mSampleRateHz / mSettings->mFrequency; // 562.5
	U32 samples_to_first_center_of_first_data_bit = U32( 1.5 * double( mSampleRateHz ) / double( mSettings->mFrequency ) );  // 843.75

	for ( ; ; )
	{
	if( mSerial->GetBitState() == BIT_HIGH )
	{
		mSerial->AdvanceToNextEdge();
	}
	tick = mSerial->GetSampleNumber();
	mSerial->AdvanceToNextEdge();
	ntick = mSerial->GetSampleNumber();

	mtick = U32((ntick - tick)*1000000/mSampleRateHz);


	if (mtick > 8700 && mtick < 9300)
	{
		// Found 9ms leading burst
		Frame frame;
		frame.mData1 = 0x1;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = tick;
		frame.mEndingSampleInclusive = ntick;
		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );

		// Look for a 4.5ms space
		mSerial->AdvanceToNextEdge();
		tick = mSerial->GetSampleNumber();

		mtick = U32((tick - ntick)*1000000/mSampleRateHz);

		if (mtick > 4250 && mtick < 4750)
		{
			// Found 4.5ms space
			Frame frame;
			frame.mData1 = 0x2;
			frame.mFlags = 0;
			frame.mStartingSampleInclusive = ntick;
			frame.mEndingSampleInclusive = tick;
			mResults->AddFrame( frame );
			mResults->CommitResults();
			ReportProgress( frame.mEndingSampleInclusive );

			mSerial->AdvanceToNextEdge();
			tick = mSerial->GetSampleNumber();

			// Decode address of receiving device
			tick = this->DecodeAddr(tick, 8, IRAnalyzerResults::IADDR);

			// Decode logical inverse of address
			tick = this->DecodeAddr(tick, 8, IRAnalyzerResults::ADDR);

			// Decode command
			tick = this->DecodeAddr(tick, 8, IRAnalyzerResults::CMD);

			// Decode logical inverse of command
			tick = this->DecodeAddr(tick, 8, IRAnalyzerResults::ICMD);

			// Space
			mSerial->AdvanceToNextEdge();
			ntick = mSerial->GetSampleNumber();
			mtick = U32((ntick - tick)*1000000/mSampleRateHz);
			if (mtick > 40500 && mtick < 42500)
			{
				Frame frame;
				frame.mData1 = 0x2;
				frame.mFlags = 0;
				frame.mStartingSampleInclusive = tick;
				frame.mEndingSampleInclusive = ntick;
				mResults->AddFrame( frame );
				mResults->CommitResults();
				ReportProgress( frame.mEndingSampleInclusive );
			}

			// Repeat
			tick = ntick;
			sample = ntick;
			mSerial->AdvanceToNextEdge();
			ntick = mSerial->GetSampleNumber();
			mtick = U32((ntick - sample)*1000000/mSampleRateHz);
			if (mtick > 8700 && mtick < 9300)
			{
				sample = ntick;
				mSerial->AdvanceToNextEdge();
				ntick = mSerial->GetSampleNumber();
				mtick = U32((ntick - sample)*1000000/mSampleRateHz);
				if (mtick > 2100 && mtick < 2400)
				{
					sample = ntick;
					mSerial->AdvanceToNextEdge();
					ntick = mSerial->GetSampleNumber();
					mtick = U32((ntick - sample)*1000000/mSampleRateHz);
					if (mtick > 550 && mtick < 620)
					{
						Frame frame;
						frame.mData1 = 0x4;
						frame.mFlags = 0;
						frame.mStartingSampleInclusive = tick;
						frame.mEndingSampleInclusive = ntick;
						mResults->AddFrame( frame );
						mResults->CommitResults();
						ReportProgress( frame.mEndingSampleInclusive );
					}
				}
			}
		}
	}
	}

	mResults->CommitResults();
}

U64 IRAnalyzer::DecodeAddr(U64 sample, U8 j, IRAnalyzerResults::TypeFrame type)
{
	U8 addr = 0xFF;
	U64 ntick, mtick, first, tt;

	first = sample;
	tt = sample;

	for( U8 i=0; i<j; i++ )
	{
		mSerial->AdvanceToNextEdge();
		ntick = mSerial->GetSampleNumber();
		mtick = U64((ntick - tt)*1000000/mSampleRateHz);
		if (mtick > 400 && mtick < 700)
		{
			addr ^= (1 << (7-i));
		}
		mSerial->AdvanceToNextEdge();
		tt = mSerial->GetSampleNumber();
	}
	Frame frame;
	frame.mData1 = j | type;
	frame.mData2 = reverse(addr);
	frame.mFlags = 0;
	frame.mStartingSampleInclusive = first;
	frame.mEndingSampleInclusive = tt;
	mResults->AddFrame( frame );
	mResults->CommitResults();
	ReportProgress( frame.mEndingSampleInclusive );

	return tt;
}



U8 IRAnalyzer::reverse(U8 b) {
   U8 s = 0x00;
   for (U8 i=1; i<=8; i++) {
	   if (b & 1) {
		   b = b >> 1;
		   s |= 1 << (8 - i);
	   }
	   else
		   b = b >> 1;
   }
   return s;
}

bool IRAnalyzer::NeedsRerun()
{
	return false;
}

U32 IRAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false)
	{

		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 IRAnalyzer::GetMinimumSampleRateHz()
{
	return mSettings->mFrequency * 4;
}

const char* IRAnalyzer::GetAnalyzerName() const
{
	return "InfraRed";
}

const char* GetAnalyzerName()
{
	return "InfraRed";
}

Analyzer* CreateAnalyzer()
{
	return new IRAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
