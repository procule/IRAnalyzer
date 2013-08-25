#include "IRAnalyzerSettings.h"
#include <AnalyzerHelpers.h>
#include <cstring>


IRAnalyzerSettings::IRAnalyzerSettings()
:	mInputChannel( UNDEFINED_CHANNEL ),
	mFrequency( 17777 ),
	mSignal (NEC_SIG)
{
	mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelInterface->SetTitleAndTooltip( "Input", "Standard InfraRed" );
	mInputChannelInterface->SetChannel( mInputChannel );

	mFrenquencyInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mFrenquencyInterface->SetTitleAndTooltip( "Frequency (Hz)",  "Specify the frequency used." );
	mFrenquencyInterface->SetMax( 6000000 );
	mFrenquencyInterface->SetMin( 1 );
	mFrenquencyInterface->SetInteger( mFrequency );

	mSignalInterface.reset( new AnalyzerSettingInterfaceNumberList() );
	mSignalInterface->SetTitleAndTooltip( "Signal", "Type of signal" );
    mSignalInterface->AddNumber( NEC_SIG, "NEC (32 bits)", "");
    mSignalInterface->SetNumber( mSignal );

	AddInterface( mInputChannelInterface.get() );
	AddInterface( mFrenquencyInterface.get() );
	AddInterface( mSignalInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannel, "Infrared", false );
}

IRAnalyzerSettings::~IRAnalyzerSettings()
{
}

bool IRAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();
	mFrequency = mFrenquencyInterface->GetInteger();
	mSignal = (IRSignal) U32( mSignalInterface->GetNumber() );

	ClearChannels();
	AddChannel( mInputChannel, "Infrared", true );

	return true;
}

void IRAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel( mInputChannel );
	mFrenquencyInterface->SetInteger( mFrequency );
	mSignalInterface->SetNumber( mSignal );
}

void IRAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	const char* name_string;
	text_archive >> &name_string;
	if( strcmp( name_string, "IRAnalyzer" ) != 0 )
			AnalyzerHelpers::Assert( "IRAnalyzer: Provided with a settings string that doesn't belong to us;" );


	text_archive >> mInputChannel;
	text_archive >> mFrequency;
	text_archive >> *(U32*)&mSignal;

	ClearChannels();
	AddChannel( mInputChannel, "Infrared", true );

	UpdateInterfacesFromSettings();
}

const char* IRAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << "IRAnalyzer";
	text_archive << mInputChannel;
	text_archive << mFrequency;
	text_archive << mSignal;

	return SetReturnString( text_archive.GetString() );
}
