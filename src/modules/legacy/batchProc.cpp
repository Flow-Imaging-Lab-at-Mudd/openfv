#include "std_include.h"

#include "calibration.h"
#include "refocusing.h"
#include "pLoc.h"
#include "tracking.h"
#include "tools.h"
#include "optimization.h"
#include "typedefs.h"

#include "batchProc.h"

batchFind::batchFind(string path): path_(path) {}

void batchFind::run() {

    read_config_file();

    cout<<"JOB PRE RUN SUMMARY:"<<endl;
    for (int i=0; i<n_; i++) {
        cout<<i+1<<") ";
        cout<<calib_paths[i]<<" ";
        cout<<refoc_paths[i]<<" ";
        cout<<threshs[i]<<" ";
        cout<<zms[i]<<" ";
        if (all_frame_flags[i]) {
            cout<<"(all frames)"<<endl;
        } else {
            cout<<"(frame "<<frames[i].x<<" to "<<frames[i].y<<")"<<endl;
        }
    }

    cout<<"Run? (y/n)";
    char ans;
    cin>>ans;

    if (ans=='y') {

    for (int i=0; i<n_; i++) {

        refocus_settings settings;
        settings.gpu = 1;
        settings.ref = 1;
        settings.mult = 0;
        settings.mult_exp = 1/9.0;
        settings.hf_method = 1;
        settings.calib_file_path = calib_paths[i];
        settings.images_path = refoc_paths[i];
        settings.mtiff = 1;
        if (all_frame_flags[i]) {
            settings.all_frames = 1;
        } else {
            settings.all_frames = 0;
            settings.start_frame = frames[i].x;
            settings.end_frame = frames[i].y;
        }

        settings.upload_frame = -1;

        int window = 2;
        double thresh = threshs[i];

        saRefocus refocus(settings);
        refocus.initializeGPU();

        localizer_settings s2;
        s2.window = window;
        s2.zmin = -5.0;
        s2.zmax = 105.0;
        s2.dz = 0.1;
        s2.thresh = thresh;
        s2.zmethod = zms[i];
        s2.show_particles = 0;

        pLocalize localizer(s2, refocus, settings);        
        localizer.find_particles_all_frames();
        localizer.write_all_particles_to_file(refoc_paths[i]);
        
    }
    
    }

}

void batchFind::read_config_file() {

    ifstream file;
    file.open(path_.c_str());

    file>>n_;

    string str;
    double t;
    int all, start, end, zm;

    // read blank line
    getline(file, str);

    for (int i=0; i<n_; i++) {

        getline(file, str);
        getline(file, str);
        calib_paths.push_back(str);
        //cout<<str<<endl;

        getline(file, str);
        refoc_paths.push_back(str);
        //cout<<str<<endl;

        file>>t;
        threshs.push_back(t);
        //cout<<t<<endl;

        file>>zm;
        zms.push_back(zm);

        file>>all;
        all_frame_flags.push_back(all);
        if (!all) {
            file>>start;
            file>>end;
            frames.push_back(Point2i(start, end));
        } else {
            frames.push_back(Point2i(0, 0));
        }

        // read blank line
        getline(file, str);
        
    }

}
