
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "CurveCalculator.h"

/////////////////////////////////////////
// BEGIN PORTED FROM SYSTEM ARCHITECH
/////////////////////////////////////////

static const float DefaultQ = 4.0f;
static const float DefaultFreq = 0.5f;
static const float DefaultGain = 0.0f;
static const float MaxQFactor = 128.0f;
static const float MinQFactor = 0.1f;
static const float FortyLN2OverLN10 = 12.04119982655924780854955578898f;

float CurveCalculator::computeGain( float frequency )		// linear
{
    float omega = computeOmega( frequency );
    float cascadedResponse = 1.0f;
	
	for (int n = 0; n < ChannelCount; n++)
	{
		Coeff & c = coeff[n];
		
        float w = (c.C * c.OmegaC * c.OmegaC - c.A * omega * omega);
        float x = (c.B * c.OmegaC * omega);
        float y = (c.F * c.OmegaC * c.OmegaC - c.D * omega * omega);
        float z = (c.E * c.OmegaC * omega);
		
        float sectionResponse = sqrt(w * w + x * x) / sqrt(y * y + z * z);
		
        sectionResponses[n] = log10f(sectionResponse);
		cascadedResponse *= sectionResponse;
	}
	
    return log10f(cascadedResponse);
}

float CurveCalculator::computeOmega( float freq )
{
    const float SampleRate = 48000.0f;		// TBD: is this valid for all devices?
    return (((float)M_PI) * 2.0f * freq ) / SampleRate;
}

float CurveCalculator::computeGainDB(float frequency)
{
    return (20.0f * log10f(computeGain(frequency)));
}

void CurveCalculator::computeCoefficients(int n)
{
	Settings & s = settings[n];
	
	if (s.filterType == 1 || s.filterType == 2)
	{
        float qq = slopeToQ( s.slope, s.gainDB );
		coeff[n] = computeCoefficients( s.gainDB, s.freq, qq, s.filterType );
		
		// Update the left and right Q frequencies
		s.leftQ = qToHz( qq, s.freq, true );
		s.rightQ = qToHz( qq, s.freq, false );
	}
	else
	{
		coeff[n] = computeCoefficients( s.gainDB, s.freq, s.Q, s.filterType );
		
		// Update the left and right Q frequencies
		s.leftQ = qToHz( s.Q, s.freq, true );
		s.rightQ = qToHz( s.Q, s.freq, false );
	}
}

float CurveCalculator::qToHz( float Q, float f0, bool LeftQ )
{
	// Get the frequency value related to the distance Q of a target filter
    float RetVal;
    float Distance;
	
	// convert Q to "half" the distance, in octaves
    Distance = sqrtf(powf(2.0f,qToOctaveBandwidth(Q)));
	
	// if Q1 then divide, otherwise multiply
	if (LeftQ)
		RetVal = f0/Distance;
	else
		RetVal = f0*Distance;
	
	return RetVal;
}

float CurveCalculator::kToQ( float K )
{
    return (powf(2.0f, 1.0f / (2.0f * K))/(powf(2.0f, (1.0f/K)) - 1.0f));
}

float CurveCalculator::qToOctaveBandwidth( float Q )
{
	if (Q >= MaxQFactor)
        return 0.0f;
	
	// Pick some test numbers
    const float kLoK = 0.000001f;
    const float kHiK = 100.0f;
    float hiK = kHiK;
    float loK = kLoK;;
    float k;
    float myQ;
	do
	{
        k = (hiK + loK) / 2.0f;	// get a mean as a test k
		myQ = kToQ(k);
		
        if (abs(myQ - Q) >= 0.0001f)	// is this really close to the passed Q?
		{
			if (myQ > Q)	// lower the ceiling or raise the floor
				hiK = k;
			else
				loK = k;
		}
		else
			break;
	} while (true);
	
	// the bandwidth is the reciprocal
    return 1.0f / k;
}

float CurveCalculator::slopeToQ( float Slope, float Gain)
{
	if (Slope == 0)
		Slope = 0.001F;
	
	if (Slope > maxSlope(Gain))
		return MaxQFactor;
	
    float g = (Gain >= 0 ? gainDBtoLinear(Gain) : gainDBtoLinear(-Gain));
    float M = (Slope >= 0 ? Slope : -Slope);
	
	if (g == 1) // Gain of 0
		return 1000; // Basically infinity.
	
//	if (M < 0)
//		HProLog.Log( 0, logType.Error, logCategory.Application, logPriority.High, DateTime.Now, "Math error in SlopeToQ()", "");
	
    float numSquared = (M * (pow(g, 2.0f) + g + 2.0f*(pow(g, 3.0f/2.0f))) );
    float denSquared =
    (FortyLN2OverLN10 * (1.0f - 2.0f*pow(g, 3.0f/2.0f) - pow(g, 2.0f) + 2.0f*pow(g, 0.5f)))
    - ((2.0f*g - g*g - 1.0f) * M);
	
    numSquared = (numSquared >= 0.0f ? numSquared : -numSquared);
    denSquared = (denSquared >= 0.0f ? denSquared : -denSquared);
	
    float num = pow(numSquared, 0.5f);
    float den = pow(denSquared, 0.5f);
    float retVal = num / den;
	
	if ( retVal != retVal)
		retVal = DefaultQ;
	else if (retVal < MinQFactor)
		retVal = MinQFactor;
	else if (retVal > MaxQFactor)
		retVal = MaxQFactor;
	
	return retVal;
}

float CurveCalculator::maxSlope( float Gain )
{
	if (Gain == 0)
        return 12.041f;
	
    float g = (Gain >= 0 ? gainDBtoLinear(Gain) : gainDBtoLinear(-Gain));
    float num = FortyLN2OverLN10 * (g - 1);
    float den = g - 2.0f*pow(g, 0.5f) + 1.0f;
    float retVal = num / den;
	return retVal;
}

float CurveCalculator::gainDBtoLinear( float gainDB )
{
	//
	// convert dB gain to linear gain multiple e.g.
	//							 0 dB implies multiply by 1 (10^0)
	// 6 db implies multiply by 2 (10^(6/20) = 1.999 )
	//
    return pow(10.0f,gainDB/20.0f);
}

CurveCalculator::Coeff CurveCalculator::computeCoefficients(float gainDB, float frequency, float Q, int filterType )
{
	const int filtersects = 1;
	
	Coeff biquadCoeffs[filtersects];
	
	// find linear gain
    float gain = pow( 10.0f, gainDB / 20.0f );
	
	int totalorder = 1;
	if (filterType == 0) totalorder = 2;
	
	for (int fs = 0; fs < filtersects; ++fs)
	{
		// Initialize
		biquadCoeffs[fs].A = 0;
		biquadCoeffs[fs].B = 0;
		biquadCoeffs[fs].C = 0;
		biquadCoeffs[fs].D = 0;
		biquadCoeffs[fs].E = 0;
		biquadCoeffs[fs].F = 0;
		biquadCoeffs[fs].OmegaC = computeOmega( frequency );
		
		
		//
		// First order section
		//
		if (totalorder == 1)
		{
			if (filterType == 1 || filterType == 2)
			{
				if (filterType == 1)
				{
					if (gain >= 1.0)	// low boost
					{
                        biquadCoeffs[fs].A = 0.0f;
                        biquadCoeffs[fs].D = 0.0f;
                        biquadCoeffs[fs].B = 1.0f;
                        biquadCoeffs[fs].E = 1.0f;
                        biquadCoeffs[fs].F = 1.0f;
						biquadCoeffs[fs].C = gain;
					}
					else	// low cut
					{
                        biquadCoeffs[fs].A = 0.0f;
                        biquadCoeffs[fs].D = 0.0f;
                        biquadCoeffs[fs].B = 1.0f;
                        biquadCoeffs[fs].C = 1.0f;
                        biquadCoeffs[fs].E = 1.0f;
                        biquadCoeffs[fs].F = 1.0f / gain;
					}
				}
				else if (filterType == 2)
				{
                    if (gain >= 1.0f)	// high boost
					{
                        biquadCoeffs[fs].A = 0.0f;
                        biquadCoeffs[fs].D = 0.0f;
                        biquadCoeffs[fs].C = 1.0f;
                        biquadCoeffs[fs].E = 1.0f;
                        biquadCoeffs[fs].F = 1.0f;
						biquadCoeffs[fs].B = gain;
					}
					else	// high cut
					{
                        biquadCoeffs[fs].A = 0.0f;
                        biquadCoeffs[fs].D = 0.0f;
                        biquadCoeffs[fs].B = 1.0f;
                        biquadCoeffs[fs].C = 1.0f;
                        biquadCoeffs[fs].F = 1.0f;
                        biquadCoeffs[fs].E = 1.0f / gain;
					}
				}
			}		// class == SHELF
			
		}
		
		//
		// Higher order section
		//
		if (totalorder > 1)
		{
			if (isAFS) {
				// code for AFS provided by Brandon Graham
				float QscaleFactor;
				float G = powf(10.0f,-gainDB*0.05f); //Convert gain from dB to linear
				if(gainDB <= -6)
					QscaleFactor = sqrt(G*G - 2);
				else
					QscaleFactor = 1; //No Q change if cutting less than 6 dB
				Q *= QscaleFactor;
			}
			
            float QRecip = 1.0f / Q;
			
			if (filterType == 1 || filterType == 2)
			{
                float sqrtGain = sqrt( gain );
				if (filterType == 1)
				{
                    if (gain >= 1.0f)	// low-boost
					{
						biquadCoeffs[fs].A = sqrtGain;
						biquadCoeffs[fs].B = gain * QRecip;
						biquadCoeffs[fs].C = gain;
						biquadCoeffs[fs].D = sqrtGain;
						biquadCoeffs[fs].E = sqrtGain * QRecip;
                        biquadCoeffs[fs].F = 1.0f;
					}
					else	// low-cut
					{
                        gain = 1.0f / gain;
                        sqrtGain = 1.0f / sqrtGain;
						
						biquadCoeffs[fs].A = sqrtGain;
						biquadCoeffs[fs].B = sqrtGain * QRecip;
						biquadCoeffs[fs].C = 1.0;
						biquadCoeffs[fs].D = sqrtGain;
						biquadCoeffs[fs].E = gain * QRecip;
						biquadCoeffs[fs].F = gain;
						
					}
				}
				else if (filterType == 2)
				{
					if (gain >= 1.0)	// high-boost
					{
						biquadCoeffs[fs].A = gain;
						biquadCoeffs[fs].B = gain * QRecip;
						biquadCoeffs[fs].C = sqrtGain;
						biquadCoeffs[fs].D = 1.0;
						biquadCoeffs[fs].E = sqrtGain * QRecip;
						biquadCoeffs[fs].F = sqrtGain;
					}
					else	// high-cut
					{
                        gain = 1.0f / gain;
                        sqrtGain = 1.0f / sqrtGain;
						
						biquadCoeffs[fs].A = 1.0;
						biquadCoeffs[fs].B = sqrtGain * QRecip;
						biquadCoeffs[fs].C = sqrtGain;
						biquadCoeffs[fs].D = gain;
						biquadCoeffs[fs].E = gain * QRecip;
						biquadCoeffs[fs].F = sqrtGain;
					}
				}
			}		// _genus == SHELF
			else if (filterType == 0)
			{
                biquadCoeffs[fs].A = 1.0f;
                biquadCoeffs[fs].C = 1.0f;
                biquadCoeffs[fs].D = 1.0f;
                biquadCoeffs[fs].F = 1.0f;
				
				if (gain >= 1.0)
				{
					biquadCoeffs[fs].B = gain * QRecip;
					biquadCoeffs[fs].E = QRecip;
				}
				else
				{
					biquadCoeffs[fs].B = QRecip;
					biquadCoeffs[fs].E = QRecip / gain;
				}
			}
			
			totalorder -= 2;
		}
		
		// prep for next section
		gain = 1.0;
	}
	
	return biquadCoeffs[0];
}

// RANGES
// q: .1 - 35 oct (bell only)
// slope 1 - 12 (shelf only)
// type: high shelf, low shelf, bell
// gain: -20dB - 20dB
// freq: 20Hz - 20kHz

/////////////////////////////////////////
// END PORTED FROM SYSTEM ARCHITECH
/////////////////////////////////////////
