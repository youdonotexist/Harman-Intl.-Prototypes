/**
 * Created by mdaly on 12/31/13.
 */

/////////////////////////////////////////
// BEGIN PORTED FROM SYSTEM ARCHITECH
/////////////////////////////////////////

var DefaultQ = 4;
var DefaultFreq = 0.5;
var DefaultGain = 0;
var MaxQFactor = 128;
var MinQFactor = 0.1;
var FortyLN2OverLN10 = 12.04119982655924780854955578898;

CurveCalculator = function(channels, afs) {
    this.ChannelCount       = channels;
    this.MaxChannelCount    = 12;
    this.isAFS              = afs;

    this.settings           = new Array(this.MaxChannelCount);
    this.coeff              = new Array(this.MaxChannelCount);
    this.sectionResponses   = new Array(this.MaxChannelCount);

    for (var i = 0; i < this.MaxChannelCount; i++) {
        this.settings[i] = {
            Q: 0,
            slope: 0,
            gainDB: 0,
            freq: 0,
            leftQ: 0,
            rightQ: 0,
            filterType: 0, // 0:bell 1:low-shelf 2:high-shelf
            filterStatus: 0 // ???
        }

        this.coeff[i] = {
            OmegaC: 0,
            A: 0,
            B: 0,
            C: 0,
            D: 0,
            E: 0,
            F: 0
        }

        this.sectionResponses[i] = 0;
    }

    this.log10 = function(val) {
        return Math.log(val) / Math.LN10;
    }

    this.computeOmega = function( freq )
    {
        var SampleRate = 48000.0;		// TBD: is this valid for all devices?
        return (Math.PI * 2.0 * freq ) / SampleRate;
    }

    this.computeGainDB = function(frequency)
    {
        return (20.0 * BA.Utils.Log10(this.computeGain(frequency)));
    }

    this.qToHz = function ( Q, f0, LeftQ )
    {
        // Get the frequency value related to the distance Q of a target filter
        var RetVal;
        var Distance;

        // convert Q to "half" the distance, in octaves

        /////////////////INLINING/////////////////////
        if (Q >= MaxQFactor)
            return 0.0;

        // Pick some test numbers
        var kLoK = 0.000001;
        var kHiK = 100.0;
        var hiK = kHiK;
        var loK = kLoK;
        var k;
        var myQ;
        do
        {
            k = (hiK + loK) / 2.0;	// get a mean as a test k
            var cachedQ = CurveCalculator.kToQCache[k];
            myQ =  cachedQ ? cachedQ : (Math.pow(2.0, 1.0 / (2.0 * k))/(Math.pow(2.0, (1.0/k)) - 1.0));
            CurveCalculator.kToQCache[k] = myQ;

            if (Math.abs(myQ - Q) >= 0.0001)	// is this really close to the passed Q?
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
        var out = 1.0 / k;
        /////////////////////////////////////////////
        Distance = Math.sqrt(Math.pow(2.0,out));

        // if Q1 then divide, otherwise multiply
        if (LeftQ)
            RetVal = f0/Distance;
        else
            RetVal = f0*Distance;

        return RetVal;
    }

    this.kToQ = function( K )
    {
        //var cached = CurveCalculator.kToQCache[K];
        //if (cached) {
        //    return cached;
        //}

        console.log("missed cache");

        var cached = (Math.pow(2.0, 1.0 / (2.0 * K))/(Math.pow(2.0, (1.0/K)) - 1.0));
        CurveCalculator.kToQCache[K] = cached;
        return cached;

    }

    this.qToOctaveBandwidth = function( Q )
    {
        if (Q >= MaxQFactor)
            return 0.0;

        // Pick some test numbers
        var kLoK = 0.000001;
        var kHiK = 100.0;
        var hiK = kHiK;
        var loK = kLoK;
        var k;
        var myQ;
        do
        {
            k = (hiK + loK) / 2.0;	// get a mean as a test k
            var cachedQ = CurveCalculator.kToQCache[k];
            myQ =  cachedQ ? cachedQ : (Math.pow(2.0, 1.0 / (2.0 * k))/(Math.pow(2.0, (1.0/k)) - 1.0));
            CurveCalculator.kToQCache[k] = myQ;

            if (Math.abs(myQ - Q) >= 0.0001)	// is this really close to the passed Q?
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
        return 1.0 / k;
    }

    this.slopeToQ = function( Slope, Gain)
    {
        if (Slope == 0)
            Slope = 0.001;

        if (Slope > this.maxSlope(Gain))
            return MaxQFactor;

        var g = (Gain >= 0 ? this.gainDBtoLinear(Gain) : this.gainDBtoLinear(-Gain));
        var M = (Slope >= 0 ? Slope : -Slope);

        if (g == 1) // Gain of 0
            return 1000; // Basically infinity.

//	if (M < 0)
//		HProLog.Log( 0, logType.Error, logCategory.Application, logPriority.High, DateTime.Now, "Math error in SlopeToQ()", "");

        var numSquared = (M * (Math.pow(g, 2.0) + g + 2*(Math.pow(g, 3.0/2.0))) );
        var denSquared =
            (FortyLN2OverLN10 * (1 - 2*Math.pow(g, 3.0/2.0) - Math.pow(g, 2.0) + 2*Math.pow(g, 0.5)))
                - ((2*g - g*g - 1) * M);

        numSquared = (numSquared >= 0 ? numSquared : -numSquared);
        denSquared = (denSquared >= 0 ? denSquared : -denSquared);

        var num = Math.pow(numSquared, 0.5);
        var den = Math.pow(denSquared, 0.5);
        var retVal = num / den;

        if ( retVal != retVal)
            retVal = DefaultQ;
        else if (retVal < MinQFactor)
            retVal = MinQFactor;
        else if (retVal > MaxQFactor)
            retVal = MaxQFactor;

        return retVal;
    }

    this.maxSlope = function( Gain )
    {
        if (Gain == 0)
            return 12.041;

        var g = (Gain >= 0 ? this.gainDBtoLinear(Gain) : this.gainDBtoLinear(-Gain));
        var num = FortyLN2OverLN10 * (g - 1);
        var den = g - 2*Math.pow(g, 0.5) + 1;
        var retVal = num / den;
        return retVal;
    }

    this.gainDBtoLinear = function( gainDB )
    {
        //
        // convert dB gain to linear gain multiple e.g.
        //							 0 dB implies multiply by 1 (10^0)
        // 6 db implies multiply by 2 (10^(6/20) = 1.999 )
        //
        return Math.pow(10.0,gainDB/20.0);
    }
}

CurveCalculator.constructor = CurveCalculator;
CurveCalculator.kToQCache = {}

CurveCalculator.prototype.computeGain = function (frequency)		// linear
{
    var omega = this.computeOmega( frequency );
    var cascadedResponse = 1.0;

    for (var n = 0; n < this.ChannelCount; n++)
    {
        //Coeff &
        var c = this.coeff[n];

        var w = (c.C * c.OmegaC * c.OmegaC - c.A * omega * omega);
        var x = (c.B * c.OmegaC * omega);
        var y = (c.F * c.OmegaC * c.OmegaC - c.D * omega * omega);
        var z = (c.E * c.OmegaC * omega);

        var sectionResponse = Math.sqrt(w * w + x * x) / Math.sqrt(y * y + z * z);

        this.sectionResponses[n] = BA.Utils.Log10(sectionResponse);
        cascadedResponse *= sectionResponse;
    }

    return BA.Utils.Log10(cascadedResponse);
}

CurveCalculator.prototype.computeCoefficients = function (n)
{
    //Settings&
    var s = this.settings[n];

    if (s.filterType == 1 || s.filterType == 2)
    {
        var qq = this.slopeToQ( s.slope, s.gainDB );
        this.coeff[n] = this.computeCoefficients2( s.gainDB, s.freq, qq, s.filterType );

        // Update the left and right Q frequencies
        s.leftQ = this.qToHz( qq, s.freq, true );
        s.rightQ = this.qToHz( qq, s.freq, false );
    }
    else
    {
        this.coeff[n] = this.computeCoefficients2( s.gainDB, s.freq, s.Q, s.filterType );

        // Update the left and right Q frequencies
        s.leftQ = this.qToHz( s.Q, s.freq, true );
        s.rightQ = this.qToHz( s.Q, s.freq, false );
    }
}

CurveCalculator.prototype.computeCoefficients2 = function ( gainDB, frequency, Q, filterType )
{
    var filtersects = 1;

    var biquadCoeffs = new Array(filtersects);

    // find linear gain
    var gain = Math.pow( 10.0, gainDB / 20.0 );

    var totalorder = 1;
    if (filterType == 0) totalorder = 2;

    for (var fs = 0; fs < filtersects; ++fs)
    {
        // Initialize
        biquadCoeffs[fs] = {A: 0};
        biquadCoeffs[fs] = {B: 0};
        biquadCoeffs[fs] = {C: 0};
        biquadCoeffs[fs] = {D: 0};
        biquadCoeffs[fs] = {E: 0};
        biquadCoeffs[fs] = {F: 0};
        biquadCoeffs[fs] = {OmegaC: this.computeOmega( frequency )}


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
                        biquadCoeffs[fs].A = 0.0;
                        biquadCoeffs[fs].D = 0.0;
                        biquadCoeffs[fs].B = 1.0;
                        biquadCoeffs[fs].E = 1.0;
                        biquadCoeffs[fs].F = 1.0;
                        biquadCoeffs[fs].C = gain;
                    }
                    else	// low cut
                    {
                        biquadCoeffs[fs].A = 0.0;
                        biquadCoeffs[fs].D = 0.0;
                        biquadCoeffs[fs].B = 1.0;
                        biquadCoeffs[fs].C = 1.0;
                        biquadCoeffs[fs].E = 1.0;
                        biquadCoeffs[fs].F = 1.0 / gain;
                    }
                }
                else if (filterType == 2)
                {
                    if (gain >= 1.0)	// high boost
                    {
                        biquadCoeffs[fs].A = 0.0;
                        biquadCoeffs[fs].D = 0.0;
                        biquadCoeffs[fs].C = 1.0;
                        biquadCoeffs[fs].E = 1.0;
                        biquadCoeffs[fs].F = 1.0;
                        biquadCoeffs[fs].B = gain;
                    }
                    else	// high cut
                    {
                        biquadCoeffs[fs].A = 0.0;
                        biquadCoeffs[fs].D = 0.0;
                        biquadCoeffs[fs].B = 1.0;
                        biquadCoeffs[fs].C = 1.0;
                        biquadCoeffs[fs].F = 1.0;
                        biquadCoeffs[fs].E = 1.0 / gain;
                    }
                }
            }		// class == SHELF

        }

        //
        // Higher order section
        //
        if (totalorder > 1)
        {
            if (this.isAFS) {
                // code for AFS provided by Brandon Graham
                var QscaleFactor;
                var G = Math.pow(10.0,-gainDB*0.05); //Convert gain from dB to linear
                if(gainDB <= -6)
                    QscaleFactor = Math.sqrt(G*G - 2);
                else
                    QscaleFactor = 1; //No Q change if cutting less than 6 dB
                Q *= QscaleFactor;
            }

            var QRecip = 1.0 / Q;

            if (filterType == 1 || filterType == 2)
            {
                var sqrtGain = sqrt( gain );
                if (filterType == 1)
                {
                    if (gain >= 1.0)	// low-boost
                    {
                        biquadCoeffs[fs].A = sqrtGain;
                        biquadCoeffs[fs].B = gain * QRecip;
                        biquadCoeffs[fs].C = gain;
                        biquadCoeffs[fs].D = sqrtGain;
                        biquadCoeffs[fs].E = sqrtGain * QRecip;
                        biquadCoeffs[fs].F = 1.0;
                    }
                    else	// low-cut
                    {
                        gain = 1.0 / gain;
                        sqrtGain = 1.0 / sqrtGain;

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
                        gain = 1.0 / gain;
                        sqrtGain = 1.0 / sqrtGain;

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
                biquadCoeffs[fs].A = 1.0;
                biquadCoeffs[fs].C = 1.0;
                biquadCoeffs[fs].D = 1.0;
                biquadCoeffs[fs].F = 1.0;

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
