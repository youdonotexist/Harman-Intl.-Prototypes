#ifndef __CURVECALCULATOR_H__
#define __CURVECALCULATOR_H__

class CurveCalculator
{
public:
	struct Settings {
        float Q;
        float slope;
        float gainDB;
        float freq;
        float leftQ, rightQ;
		int filterType; // 0:bell 1:low-shelf 2:high-shelf
		int filterStatus; // ???
	};
	
	struct Coeff {
        float OmegaC;
        float A, B, C, D, E, F;
	};
		
	int ChannelCount;
	static const int MaxChannelCount = 12;
	bool isAFS;
	
	Settings settings[MaxChannelCount];
		
	Coeff coeff[MaxChannelCount];
	
	float sectionResponses[MaxChannelCount];

	// PORTED FROM SYSTEM ARCHITECH
    float computeGain( float frequency );
    float computeOmega( float freq );
    float computeGainDB(float frequency);
	void computeCoefficients(int n);
    float qToHz( float Q, float f0, bool LeftQ );
    float kToQ( float K );
    float qToOctaveBandwidth( float Q );
    float slopeToQ( float Slope, float Gain);
    float maxSlope( float Gain );
    float gainDBtoLinear( float gainDB );
    Coeff computeCoefficients(float gainDB, float frequency, float Q, int filterType );
};

#endif // __CURVECALCULATOR_H__
