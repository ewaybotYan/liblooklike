/**
 * @file   test_matrix.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for Matrix class
 */

#include "../include/exception.h"
#include "../include/context.h"
#include "../include/mathexpression.h"
#include "../include/matrix.h"

#include<iostream>
#include<cmath>

using namespace std;

const int n = 50;
const int m = 5;
float data[n*m] = {
    0.811998870, 0.281545461, 0.08758553, 0.35107806, 0.9086247343,
    0.206221032, -0.162219924, 0.29274794, -0.16393098, 0.1628313907,
    -0.635741870, -0.846542720, -0.42432402, -0.17773666, 0.3146556406,
    -0.792682657, -0.115250280, -0.21601851, -0.86114453, -0.5852674209,
    0.363440761, 0.655866757, -0.44696948, -0.21565788, -0.8667471027,
    -0.001679151, -0.631697911, -0.91941310, -0.32770824, 0.3393766708,
    -0.366340618, 0.387762762, -0.19129516, 0.85474126, 0.1621996141,
    0.944820990, -0.261192054, 0.63080185, 0.37276290, -0.3165206774,
    0.893529481, -0.015060918, -0.13905006, 0.94433397, 0.0001186416,
    -0.553556643, -0.997019464, -0.94164052, -0.28231552, -0.1889447542,
    0.580162840, -0.437816740, 0.40208152, -0.38977530, -0.7878178181,
    -0.675888291, 0.006098781, 0.08203938, -0.95854418, -0.4804540137,
    -0.415505002, -0.547149641, 0.93943534, -0.21093237, 0.9499139469,
    0.005377568, -0.330256518, -0.11186948, -0.94335261, 0.2320095617,
    0.202567255, 0.198078747, 0.67698099, -0.72887653, 0.9516922780,
    0.050603078, -0.011131367, -0.52034603, 0.08696991, 0.5740391412,
    0.892876282, -0.445913651, 0.28695284, 0.30538834, -0.2667495008,
    0.807609408, -0.206159072, -0.24431500, -0.08945015, 0.3323745215,
    -0.763695745, -0.690737626, -0.94501456, -0.75976506, -0.2316384185,
    0.153577088, -0.509917009, -0.89529851, 0.82533571, -0.1975574465,
    0.772008400, -0.799889672, 0.10419669, -0.71872302, 0.7243997692,
    -0.127837576, 0.842068471, 0.26670834, -0.32884550, 0.4243910550,
    -0.120256718, 0.763802582, -0.63597593, 0.99538247, 0.4187000296,
    0.766441896, -0.952472863, 0.72310851, 0.67600357, -0.1756840711,
    -0.601146976, -0.034099455, -0.88141529, -0.12183441, 0.8857932934,
    0.514596279, 0.173126347, -0.35448111, -0.57835277, -0.7417417923,
    -0.560101586, -0.249609279, 0.88680677, -0.59748463, -0.5592294405,
    0.532911191, 0.629949672, 0.98276246, 0.07175370, 0.5086344606,
    0.231158002, 0.290716459, -0.24023925, -0.90953457, 0.2598536280,
    0.654371232, -0.584983271, 0.72875048, -0.58636720, 0.4394802647,
    -0.830706875, 0.895405202, -0.06125433, -0.32618004, 0.1122520086,
    0.721875892, 0.135367971, 0.36290015, 0.72288221, -0.2160034971,
    -0.063006360, 0.683676311, 0.90535218, 0.10709688, -0.7751864144,
    0.775094302, -0.087666378, -0.32681383, 0.30045781, -0.5175784407,
    0.657193352, -0.395019682, -0.57074150, 0.04941351, 0.5512808617,
    -0.332022439, 0.305323680, -0.13593228, -0.08527392, -0.8913819720,
    0.931173527, -0.186632588, 0.37563117, -0.72065237, 0.0294940588,
    -0.214036278, -0.863421935, -0.96103861, 0.78033125, 0.2277282053,
    0.161477031, -0.574929394, -0.93082286, -0.48320446, 0.5500942231,
    0.486369818, 0.522273135, 0.19138912, 0.91495449, 0.9553486132,
    -0.526189411, -0.544177059, 0.93270472, 0.73475002, 0.4641960538,
    0.814602885, 0.310849317, -0.69653566, 0.60582388, 0.5099171312,
    -0.016439430, -0.543815471, -0.94091241, -0.64711261, -0.7799350647,
    0.467103092, -0.060296382, 0.18277069, 0.74472834, 0.0379397161,
    -0.068250815, -0.140890948, 0.81501404, 0.06581432, 0.4854035331,
    0.467771550, -0.049525553, -0.93150188, 0.47969591, -0.9417947098,
    -0.408082333, 0.705959840, -0.77457626, -0.55757334, 0.3656863444,
    -0.043398397, 0.303872828, -0.24492855, -0.91202298, -0.1118026045,
    -0.992069447, -0.842440675, -0.18106360, -0.77115060, 0.5393739357,
    0.456738943, -0.059428453, -0.02814885, 0.56923364, -0.8056881218
};

float normalized[n*m] = {
    0.174739941,   0.107624222,   0.039518004,   0.097140867,   0.218283838,
    0.020815449,   -0.016991370,  0.087694463,   -0.025187151,  0.026540378,
    -0.193122241,  -0.209158882,  -0.080689150,  -0.028466358,  0.065574391,
    -0.232999947,  -0.003801630,  -0.031774624,  -0.190793471,  -0.165795818,
    0.060764033,   0.212738932,   -0.086006780,  -0.037473634,  -0.238164241,
    -0.032010732,  -0.148827424,  -0.196946502,  -0.064088503,  0.071930167,
    -0.124669009,  0.137451541,   -0.025969061,  0.216773952,   0.026377948,
    0.208489239,   -0.044784143,  0.167076646,   0.102291578,   -0.096701033,
    0.195456377,   0.024332959,   -0.013700811,  0.238054546,   -0.015293068,
    -0.172239471,  -0.251414878,  -0.202165969,  -0.053306554,  -0.063901265,
    0.115831805,   -0.094382851,  0.113368291,   -0.078831040,  -0.217871523,
    -0.203323204,  0.030274901,   0.038215651,   -0.213928416,  -0.138848292,
    -0.137161382,  -0.125085074,  0.239550295,   -0.036351203,  0.228899294,
    -0.030217662,  -0.064178420,  -0.007318236,  -0.210320024,  0.044326085,
    0.019887047,   0.084185589,   0.177920483,   -0.159376388,  0.229356503,
    -0.018726133,  0.025436432,   -0.103237142,  0.034408324,   0.132261886,
    0.195290403,   -0.096656578,  0.086333652,   0.086288370,   -0.083904864,
    0.173624605,   -0.029330104,  -0.038419237,  -0.007496022,  0.070129915,
    -0.225634548,  -0.165406610,  -0.202958265,  -0.166713215,  -0.074877806,
    0.007438941,   -0.114629625,  -0.191283887,  0.209789370,   -0.066115588,
    0.164578604,   -0.196058047,  0.043418655,   -0.156964666,  0.170919601,
    -0.064066826,  0.265027004,   0.081579816,   -0.064358631,  0.093787364,
    -0.062140576,  0.243048837,   -0.130389479,  0.250179891,   0.092324202,
    0.163164189,   -0.238905563,  0.188752195,   0.174319109,   -0.060491944,
    -0.184331887,  0.018986668,   -0.188023815,  -0.015188124,  0.212413874,
    0.099171738,   0.077178602,   -0.064288562,  -0.123623087,  -0.206025377,
    -0.173902501,  -0.041531536,  0.227191998,   -0.128167399,  -0.159101453,
    0.103825446,   0.205461048,   0.249724417,   0.030794079,   0.115446343,
    0.027151783,   0.110199567,   -0.037462165,  -0.202287362,  0.051484794,
    0.134687710,   -0.135709292,  0.190077049,   -0.125526721,  0.097666800,
    -0.242661671,  0.280004712,   0.004567267,   -0.063725515,  0.013536419,
    0.151840236,   0.066575517,   0.104167681,   0.185454003,   -0.070858067,
    -0.047593605,  0.220548247,   0.231546851,   0.039189000,   -0.214623989,
    0.165362713,   0.003944319,   -0.057791701,  0.085117242,   -0.148392982,
    0.135404793,   -0.082364832,  -0.115071059,  0.025487704,   0.126410732,
    -0.115948967,  0.114301482,   -0.012968689,  -0.006504058,  -0.244497865,
    0.205021502,   -0.023846791,  0.107157192,   -0.157422935,  -0.007740649,
    -0.085969396,  -0.213898804,  -0.206721048,  0.199099642,   0.043225348,
    0.009446269,   -0.132886022,  -0.199625753,  -0.101022893,  0.126105647,
    0.091999565,   0.175223955,   0.063893274,   0.231076157,   0.230296546,
    -0.165285622,  -0.124250331,  0.237969803,   0.188272918,   0.104021229,
    0.175401605,   0.115853158,   -0.144610180,  0.157649611,   0.115776118,
    -0.035761230,  -0.124148792,  -0.201994992,  -0.139955333,  -0.215844867,
    0.087104006,   0.011630201,   0.061869486,   0.190643027,   -0.005569270,
    -0.048926190,  -0.011001892,  0.210333553,   0.029383322,   0.109473672,
    0.087273857,   0.014654802,   -0.199785203,  0.127690945,   -0.257458980,
    -0.135275327,  0.226805777,   -0.162935757,  -0.118687432,  0.078694384,
    -0.042611340,  0.113894062,   -0.038563312,  -0.202878423,  -0.044068019,
    -0.283662927,  -0.208006970,  -0.023566474,  -0.169417577,  0.123349462,
    0.084470538,   0.011873928,   0.012341131,   0.148958480,   -0.222465978
};

const int aw = 20;
const int ah = 5;

float a[aw*ah] = {
  0.859555,-0.03640075,0.6000662,-0.5373392,-0.3938806,
  0.6738222,0.7656746,-0.04991303,0.02807761,-0.4494087,
  0.2088377,-0.443116,0.7175984,0.3928747,0.6738882,
  0.9615358,-0.584719,-0.8826968,-0.6712232,0.3820503,
  -0.7648916,0.8049297,-0.9832899,0.5130607,0.6799698,
  0.04960564,-0.7604051,0.7494427,-0.4499969,0.3272896,
  -0.5214519,-0.6111996,-0.2553094,0.02527038,0.9416582,
  -0.1265489,-0.2117826,0.1951463,-0.1326128,-0.4972729,
  0.4347242,-0.8319752,-0.7967115,0.9188513,-0.5115155,
  0.9872034,-0.08862734,-0.526807,-0.8525717,0.4018377,
  0.3042503,-0.3617497,-0.4929073,-0.5827707,0.3516664,
  0.03433609,0.8897263,-0.8111832,-0.4255632,-0.5449469,
  0.7135514,0.1829167,0.4542794,0.1631866,-0.8150671,
  0.627327,0.3003503,-0.5743717,-0.04975398,0.5319428,
  0.4314376,-0.749185,-0.3976288,0.5695818,0.3604919,
  0.5780151,-0.1797865,0.1222054,0.5170793,-0.1334166,
  -0.2427173,0.5936625,-0.2806849,-0.5019634,0.008278464,
  -0.2818211,-0.537629,0.7154435,0.3202205,-0.9462456,
  0.3479562,-0.9487649,-0.936046,-0.4562833,-0.6306744,
  -0.408071,0.4703855,-0.2602751,0.8684106,-0.686391 
};

const int bw = 3;
const int bh = 20;

float b[bw*bh] = {
  -0.8109678,0.4771343,-0.1241322,-0.4850215,0.3545739,
  -0.1954033,-0.723628,0.3272067,0.2350641,0.995608,
  0.6095526,0.547823,0.321548,-0.1527461,-0.1135489,
  0.3035452,-0.1496363,0.9774102,-0.7570364,0.6717283,
  -0.4070171,-0.6013085,0.1414013,-0.157462,0.1706753,
  -0.7331206,0.574413,0.7305363,0.6330202,-0.5820638,
  0.2125267,-0.315942,0.6787913,0.6091844,-0.5477502,
  0.3490736,-0.6451745,0.5556291,-0.949415,0.8641615,
  0.5162845,-0.342827,0.1364762,-0.4772968,-0.3141688,
  -0.4461043,-0.832453,0.8003385,-0.7543328,0.5924721,
  -0.9466763,0.7425793,-0.5374306,-0.5069781,-0.3524633,
  0.2532249,0.3560301,-0.7638707,-0.1731585,0.7326497
};

const int cw = 3;
const int ch = 5;

float c[ah*bw] = {
  -0.04008873,1.986873,-0.2833796,1.182132,-1.969775,
  -1.532436,0.2200845,0.7200324,3.421073,-0.7900933,
  -0.6098414,3.040098,0.8856889,-0.8036001,-1.455227
};
  
void usage() {
    cerr << "Error in " << __FILE__ << " :\n"
    << "usage :\n test_matrix path_to_kernel_directory"
    << endl;
}

int main ( int argc, char* argv[] ) {
    if ( argc !=2 ) {
        usage();
        return -1;
    }
    std::string path ( argv[1] );

    try {

        Context ctx ( path );
        cl::CommandQueue queue = ctx.createQueue();
	float error;
	
	// test normalization
	cout << "testing normalization\n";
	Matrix dat ( data, m, n );
	Matrix nor = Matrix::normalize(dat);
	nor.evaluate( ctx, queue );
	nor.retrieveData( ctx, queue );
	float* res = nor.getValues();

	for( int k = 0; k < m*n; k++ ){
	  error = max( error, abs( res[k] - normalized[k] ) );
	}
        if ( error < 0.0000001 ) {
            std::cout << "computed result matches with error!\n";
        } else {
            std::cerr << "computed without noticeable error.\n";
            return -1;
        }
        
        // test multiplication
        cout << "testing multiplication\n";
        Matrix A( a, ah, aw );
	Matrix B( b, bh, bw );
	Matrix C( c, ch, cw );
	C.evaluate( ctx, queue );
	C.retrieveData( ctx, queue );
	float* res2 = C.getValues();
	
	for( int k = 0; k < ch*cw; k++ ){
	  error = max( error, abs( res2[k] - c[k] ) );
	}
        if ( error < 0.0000001 ) {
            std::cout << "computed result matches with error!\n";
        } else {
            std::cerr << "computed without noticeable error.\n";
            return -1;
        }
	
    } catch ( Error& err ) {
        err.printMsg();
	return -1;
    }
    return 0;
}

