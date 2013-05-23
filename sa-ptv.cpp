#include "std_include.h"

#include "calibration.h"
#include "refocusing.h"
#include "pLoc.h"
#include "tracking.h"
#include "tools.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

    // Camera Calibration Section
    
    string calib_path("../../experiment/calibration_full/"); // Folder where calibration images lie
    Size grid_size = Size(6,5); // Format (horizontal_corners, vertical_corners)
    double grid_size_phys = 5;  // in [mm]

    //double wall_timer;
    //wall_timer = omp_get_wtime();

    multiCamCalibration calibration(calib_path, grid_size, grid_size_phys);
    calibration.run();
    //calibration.write_calib_results_matlab();

    int frame = 0;
    int mult = 1;
    double mult_exp = 1.0/9.0;
    string refoc_path("../../experiment/calibration_full/");
    saRefocus refocus(calibration.refocusing_params(), frame, mult, mult_exp);
    //refocus.read_imgs(refoc_path);
    
    int live = 0;
    if (live) {
        refocus.GPUliveView(); 
    } else {
        refocus.initializeGPU();
        
        int window = 2;
        int cluster_size = 10;
        double zmin = -50.0; //-20
        double zmax = 50.0; //40
        double dz = 0.1;
        double thresh = 90.0; //100.0
        pLocalize localizer(window, zmin, zmax, dz, thresh, cluster_size, refocus);

        localizer.z_resolution();
        //localizer.crop_focus();
        
        //localizer.run();
        //localizer.find_particles_all_frames();

        //localizer.write_all_particles_to_file(particle_file);        
        

    }

    string particle_file("../temp/vortex_particles.txt");
    pTracking tracker(refocus);
    tracker.read_points(particle_file);
    tracker.track_all();

    //cout<<endl<<"TIME TAKEN: "<<(omp_get_wtime()-wall_timer)<<" seconds"<<endl;
    
    return 1;

}
