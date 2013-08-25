#ifndef IR_ANALYZER_H
#define IR_ANALYZER_H

#include <Analyzer.h>
#include "IRAnalyzerResults.h"
#include "IRSimulationDataGenerator.h"
#include <cassert>

#define CHAR_BITS 8


class IRAnalyzerSettings;
class ANALYZER_EXPORT IRAnalyzer : public Analyzer
{
public:
	IRAnalyzer();
	virtual ~IRAnalyzer();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();
	U64 DecodeAddr(U64 sample, U8 j, IRAnalyzerResults::TypeFrame type);
	U8 reverse(U8 b);

protected: //vars
	std::auto_ptr< IRAnalyzerSettings > mSettings;
	std::auto_ptr< IRAnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	IRSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //IR_ANALYZER_H
