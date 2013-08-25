#ifndef IR_SIMULATION_DATA_GENERATOR
#define IR_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>
#include <SimulationChannelDescriptor.h>
#include <string>
class IRAnalyzerSettings;

class IRSimulationDataGenerator
{
public:
	IRSimulationDataGenerator();
	~IRSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, IRAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	IRAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;
	U64 mValue;

protected:
	void CreateIRSignal();
	void SendOne();
	void SendZero();
	std::string mIRText;
	U32 mStringIndex;

	ClockGenerator mClockGenerator;

	SimulationChannelDescriptor mIRSimulationData;

};
#endif //IR_SIMULATION_DATA_GENERATOR
