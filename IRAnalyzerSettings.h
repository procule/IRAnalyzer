#ifndef IR_ANALYZER_SETTINGS
#define IR_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

typedef enum IRSignal { NEC_SIG };

class IRAnalyzerSettings : public AnalyzerSettings
{
public:
	IRAnalyzerSettings();
	virtual ~IRAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 mFrequency;
	IRSignal mSignal;


protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mFrenquencyInterface;
	std::auto_ptr< AnalyzerSettingInterfaceNumberList > mSignalInterface;
};

#endif //IR_ANALYZER_SETTINGS
