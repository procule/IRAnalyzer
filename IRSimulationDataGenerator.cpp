#include "IRSimulationDataGenerator.h"
#include "IRAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

#include <iostream>

IRSimulationDataGenerator::IRSimulationDataGenerator()
:	mIRText( "My first analyzer, woo hoo!" ),
	mStringIndex( 0 )
{
}

IRSimulationDataGenerator::~IRSimulationDataGenerator()
{
}

void IRSimulationDataGenerator::Initialize( U32 simulation_sample_rate, IRAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mIRSimulationData.SetChannel(settings->mInputChannel);
	mIRSimulationData.SetSampleRate( simulation_sample_rate );
	mIRSimulationData.SetInitialBitState( BIT_HIGH );


	
	
	mValue = 0;
}

U32 IRSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mIRSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		CreateIRSignal();
	}

	*simulation_channel = &mIRSimulationData;
	return 1;
}
/*
void IRSimulationDataGenerator::CreateSerialByte()
{
	U32 samples_per_bit = mSimulationSampleRateHz / mSettings->mFrequency;

	U8 byte = mIRText[ mStringIndex ];
	mStringIndex++;
	if( mStringIndex == mIRText.size() )
		mStringIndex = 0;

	//we're currenty high
	//let's move forward a little
	mIRSimulationData.Advance( samples_per_bit * 10 );

	mIRSimulationData.Transition();  //low-going edge for start bit
	mIRSimulationData.Advance( samples_per_bit );  //add start bit time

	U8 mask = 0x1 << 7;
	for( U32 i=0; i<8; i++ )
	{
		if( ( byte & mask ) != 0 )
			mIRSimulationData.TransitionIfNeeded( BIT_HIGH );
		else
			mIRSimulationData.TransitionIfNeeded( BIT_LOW );

		mIRSimulationData.Advance( samples_per_bit );
		mask = mask >> 1;
	}

	mIRSimulationData.TransitionIfNeeded( BIT_HIGH ); //we need to end high

	//lets pad the end a bit for the stop bit:
	mIRSimulationData.Advance( samples_per_bit );
}
*/

void IRSimulationDataGenerator::SendOne() {
	U32 samples_per_bit = mSimulationSampleRateHz / mSettings->mFrequency;
	mIRSimulationData.Advance( samples_per_bit * 1);
	mIRSimulationData.Transition();
	mIRSimulationData.Advance( samples_per_bit * 2);
	mIRSimulationData.Transition();
}

void IRSimulationDataGenerator::SendZero() {
	U32 samples_per_bit = mSimulationSampleRateHz / mSettings->mFrequency;
	mIRSimulationData.Advance( samples_per_bit * 1);
	mIRSimulationData.Transition();
	mIRSimulationData.Advance( samples_per_bit * 1);
	mIRSimulationData.Transition();
}
void IRSimulationDataGenerator::CreateIRSignal()
{
	U32 samples_per_bit = mSimulationSampleRateHz / mSettings->mFrequency;

	mIRSimulationData.Advance( samples_per_bit * 10 );
	mIRSimulationData.Transition();

	mIRSimulationData.Advance( samples_per_bit * 16);
	mIRSimulationData.Transition();
	mIRSimulationData.Advance( samples_per_bit * 8);
	mIRSimulationData.Transition();

	// 0000
	SendZero();
	SendZero();
	SendZero();
	SendZero();
	// 0000
	SendZero();
	SendZero();
	SendZero();
	SendZero();

	// 1111
	SendOne();
	SendOne();
	SendOne();
	SendOne();
	// 0111
	SendZero();
	SendOne();
	SendOne();
	SendOne();
	// 0100
	SendZero();
	SendOne();
	SendZero();
	SendZero();
	// 0000
	SendZero();
	SendZero();
	SendZero();
	SendZero();
	// 1011
	SendOne();
	SendZero();
	SendOne();
	SendOne();
	// 1111
	SendOne();
	SendOne();
	SendOne();
	SendOne();

	// Finalize
	mIRSimulationData.Advance( samples_per_bit * 1);
	mIRSimulationData.Transition();

	mIRSimulationData.Advance( samples_per_bit * 100);
	mIRSimulationData.Transition();
	mIRSimulationData.Transition();

}
