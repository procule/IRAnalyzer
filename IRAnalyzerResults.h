#ifndef IR_ANALYZER_RESULTS
#define IR_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class IRAnalyzer;
class IRAnalyzerSettings;

class IRAnalyzerResults : public AnalyzerResults
{
public:
	IRAnalyzerResults( IRAnalyzer* analyzer, IRAnalyzerSettings* settings );
	virtual ~IRAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

	enum TypeFrame { ADDR = 0x10 , CMD = 0x20, ICMD = 0x40 , IADDR = 0x80 };

protected: //functions

protected:  //vars
	IRAnalyzerSettings* mSettings;
	IRAnalyzer* mAnalyzer;
};

#endif //IR_ANALYZER_RESULTS
