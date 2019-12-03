#include <cstdio>
#include "firmware/linpuppi.h"
#include "../utils/DiscretePFInputsReader.h"
#include "../utils/pattern_serializer.h"
#include "../utils/test_utils.h"

#define NTEST 2


int main() {

    //DiscretePFInputsReader inputs("TTbar_PU200_HGCalNoTK.dump");
    DiscretePFInputsReader inputs("VBFHToBB_PU200_HGCalNoTK.dump");
    
    // input TP objects (used)
    HadCaloObj calo[NCALO];

    // input TP objects (unused, but needed to read the inputs)
    EmCaloObj emcalo[NEMCALO]; TkObj track[NTRACK]; MuObj mu[NMU]; z0_t hwZPV;

    // input/output PFPUPPI objects
    PFNeutralObj outselne_ref[NNEUTRALS];
    PFNeutralObj outselne_flt[NNEUTRALS];
    PFNeutralObj outallne_ref[NCALO];
    PFNeutralObj outallne_flt[NCALO];
#ifdef TEST_PUPPI_PT
    pt_t         outPuppiPt_hw[NCALO];
#elif defined(TEST_PUPPI_NOCROP)
    PFNeutralObj outallne[NCALO];
#else
    PFNeutralObj outselne[NNEUTRALS];
#endif


    int flt_nok = 0, flt_1bit = 0, flt_almostok = 0, flt_nz = 0, flt_nmiss = 0;
    float flt_sumDiff, flt_sumAbsDiff = 0;
    for (int test = 1; test <= NTEST; ++test) {
        // get the inputs from the input object
        if (!inputs.nextRegion(calo, emcalo, track, mu, hwZPV)) break;

#ifdef TEST_PT_CUT
        float minpt = 0;
        for (unsigned int i = 0; i < NCALO; ++i) minpt += calo[i].hwPt*0.25;
        if (minpt < TEST_PT_CUT) { 
            //std::cout << "Skipping region with total calo pt " << minpt << " below threshold." << std::endl; 
            --test; continue; 
        }
#endif
 
#ifdef TEST_PUPPI_PT
        fwdlinpuppiPt_hw(calo, outPuppiPt_hw);
#elif defined(TEST_PUPPI_NOCROP)
        fwdlinpuppiNoCrop_hw(calo, outallne);
#else
        fwdlinpuppi_hw(calo, outselne);
#endif
        fwdlinpuppi_ref(calo, outallne_ref, outselne_ref);
        fwdlinpuppi_flt(calo, outallne_flt, outselne_flt);

        for (int i = 0; i < NCALO; ++i){
            if (calo[i].hwPt > 0) {
                if (outallne_flt[i].hwPtPuppi > 2/0.25) flt_nz++;
                if (outallne_flt[i].hwPtPuppi == outallne_ref[i].hwPtPuppi) {
                    flt_nok++; 
                } else {
                    if (std::abs<int>(outallne_flt[i].hwPtPuppi - outallne_ref[i].hwPtPuppi)*0.25 < 1 + 0.01 * outallne_flt[i].hwPtPuppi) {
                        if (std::abs<int>(outallne_flt[i].hwPtPuppi - outallne_ref[i].hwPtPuppi) == 1) {
                            flt_1bit++; 
                        } else {
                            flt_almostok++;
                        }
                    }
                    flt_nmiss++;
                }
                float ptDiff = (outallne_flt[i].hwPtPuppi - outallne_ref[i].hwPtPuppi) * 0.25;
                flt_sumDiff += ptDiff;
                flt_sumAbsDiff += std::abs(ptDiff);
                printf("particle %02d pT %7.2f  em %1d :  puppiPt_ref %7.2f   puppiPt_flt %7.2f    diff %+7.2f\n",
                    i, calo[i].hwPt * 0.25, int(calo[i].hwIsEM), outallne_ref[i].hwPtPuppi * 0.25, outallne_flt[i].hwPtPuppi * 0.25, ptDiff);
            }
#ifdef TEST_PUPPI_PT
            if (outPuppiPt_hw[i] != outallne_ref[i].hwPtPuppi) {
                printf("MISMATCH!\n");
                printf("particle %02d pT %7.2f  em %1d :  puppiPt_hw %7.2f    puppiPt_ref %7.2f   puppiPt_flt %7.2f\n",
                    i, calo[i].hwPt * 0.25, int(calo[i].hwIsEM), outPuppiPt_hw[i] * 0.25, outallne_ref[i].hwPtPuppi * 0.25, outallne_flt[i].hwPtPuppi * 0.25);
                return 1;
            }
#elif defined(TEST_PUPPI_NOCROP)
            // apply pT cut to the reference
            if (outallne_ref[i].hwPtPuppi < 4*4) clear(outallne_ref[i]);
            if (!puppi_equals(outallne_ref[i], outallne[i], "Puppi", i)) {
                printf("particle %02d pT %7.2f  em %1d :  puppiPt_hw %7.2f    puppiPt_ref %7.2f   puppiPt_flt %7.2f\n",
                    i, calo[i].hwPt * 0.25, int(calo[i].hwIsEM), outallne[i].hwPtPuppi * 0.25, outallne_ref[i].hwPtPuppi * 0.25, outallne_flt[i].hwPtPuppi * 0.25);
                return 1;
            }
#else
            if (i <= NSELCALO && !puppi_equals(outselne_ref[i], outselne[i], "Puppi", i)) {
                printf("particle %02d pT %7.2f  em %1d :  puppiPt_hw %7.2f    puppiPt_ref %7.2f   puppiPt_flt %7.2f\n",
                    i, calo[i].hwPt * 0.25, int(calo[i].hwIsEM), outselne[i].hwPtPuppi * 0.25, outselne_ref[i].hwPtPuppi * 0.25, outselne_flt[i].hwPtPuppi * 0.25);
                return 1;
            }
#endif
        }

        printf("\n");

    }

    printf("Integer vs floating point accuracy (%d non-empty candidates out of %d regions cropped at NCALO=%d, %d candidates after PUPPI pT > 2):\n", flt_nok+flt_nmiss, NTEST, NCALO, flt_nz);
    printf("  - exact match: %6d  (%6.2f%% ):\n", flt_nok,   flt_nok * 100.0 / (flt_nok+flt_nmiss));
    printf("  - mismatch   : %6d  (%6.2f%% ):\n", flt_nmiss, flt_nmiss * 100.0 / (flt_nok+flt_nmiss));
    printf("  -   by 1*LSB : %6d  (%6.2f%% )   [ 1 unit, 0.25 GeV ]\n", flt_1bit, flt_1bit * 100.0 / (flt_nok+flt_nmiss));
    printf("  -      small : %6d  (%6.2f%% )   [ 0.25 < delta(pt) <= 1 GeV + 1% ]\n", flt_almostok, flt_almostok * 100.0 / (flt_nok+flt_nmiss));
    printf("  -      big   : %6d  (%6.2f%% )   [ delta(pt) > 1 GeV + 1% ]\n", (flt_nmiss-flt_almostok-flt_1bit), (flt_nmiss-flt_almostok-flt_1bit) * 100.0 / (flt_nok+flt_nmiss));
    printf("  - average pT  diff   %+8.4f  (on all)    %+8.4f  (on mismatch)\n", flt_sumDiff/(flt_nok+flt_nmiss), flt_sumDiff/std::max(flt_nmiss,1));
    printf("  - average pT |diff|  % 8.4f  (on all)    % 8.4f  (on mismatch)\n", flt_sumAbsDiff/(flt_nok+flt_nmiss), flt_sumAbsDiff/std::max(flt_nmiss,1));
    return 0;
}