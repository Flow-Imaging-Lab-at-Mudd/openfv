//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//                           License Agreement
//                For Open Source Flow Visualization Library
//
// Copyright 2013-2015 Abhishek Bajpayee
//
// This file is part of openFV.
//
// openFV is free software: you can redistribute it and/or modify it under the terms of the 
// GNU General Public License as published by the Free Software Foundation, either version 
// 3 of the License, or (at your option) any later version.
//
// openFV is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with openFV. 
// If not, see http://www.gnu.org/licenses/.

// -------------------------------------------------------
// -------------------------------------------------------
// Synthetic Aperture - Particle Tracking Velocimetry Code
// --- Calibration Library ---
// -------------------------------------------------------
// Author: Abhishek Bajpayee
//         Dept. of Mechanical Engineering
//         Massachusetts Institute of Technology
// -------------------------------------------------------
// -------------------------------------------------------

#ifndef CALIBRATION_LIBRARY
#define CALIBRATION_LIBRARY

#include "std_include.h"
#include "optimization.h"
#include "typedefs.h"

class camCalibration {

 public:
    
    ~camCalibration() {
        //
    }

    camCalibration() {
        //
    }

    // Function declarations
    void calibrateCamera(vector< vector<Point2f> > corner_points, Size img_size, Mat &K, Mat &rvec, Mat &tvec);

 protected:

    // Inheritable stuff

 private: 

    // Private functions and variables

};

/*!
    Class with functions that allow user to calibrate multiple cameras via bundle
    adjustment. Contains functionality for both pinhole and refractive scenes. Note
    that refractive calibration ability is currently of limited robustness.
*/
class multiCamCalibration {
 
 public:
    ~multiCamCalibration() {
        //
    }

    /*! multiCamCalibration constructor
      \param path Path to where calibration images lie. If the images are in multipage tiff files then
      this parameters just points to a folder where all (from all cameras) the multipage tiff files lie.
      If the images are in separate folders then the expected directory structure is:
      \verbatim embed:rst
      path/
      
        cam1/
          img1.jpg, img2.jpg, ...
        cam2/
          img1.jpg, img2.jpg, ...

        ...
      \endverbatim
      where cam1, cam2 etc. and img1, img2 etc. are variable names and can be of user's choice. Supported
      image types are currently limited but JPEG and TIFF are supported.
      \param grid_size The number of internal corners in the grid used (x, y)
      \param grid_size_phys Physical size of one square in the grid. Usually in [mm].
      \param refractive Flag indicating whether data set is for a refractive set or not
      \param dummy_mode 
      \param mtiff Flag indicating whether images are in multipage tiff files or as separate images
      \param skip Number of frames to skip between each successive frame used
      \param show_corners Flag to enable displaying of corners in each image after they are found
    */
    multiCamCalibration(string path, Size grid_size, double grid_size_phys, int refractive, int dummy_mode, int mtiff, int skip, int show_corners);

    // Set functions
    //! Set maximum iterations for the bundle adjustment optimization. Default is 100.
    void set_max_iterations(int num) { pinhole_max_iterations = num; }
    //! Set initial value for f (focal length) in pixel units for P matrix of cameras. Default is 2500.
    void set_init_f_value(float val) { init_f_value_ = val; }
    
    // Function to run calibration
    /*! Run a calibration job. This automatically calls functions to
        read camera names and calibration images (read_cam_names_mtiff(), read_calib_imgs_mtiff() or
        read_cam_names(), read_calib_imgs()), find_corners() function, functions to calibrate the 
        cameras, functions to write calibration results to a file. Calling only this function after
        creating a multiCamCalibration instance should typically suffice to do everything.
    */
    void run();
    
    // Functions to work with multipage tiff files
    //! Read camera names if calibration images are in multipage tiff files
    void read_cam_names_mtiff();
    //! Read images if calibration images are in multipage tiff files
    void read_calib_imgs_mtiff();

    // Functions to work with normal directory structure
    //! Read camera names if calibration images are in folders as separate images
    void read_cam_names();
    //! Read images if calibration images are in folders as separate images
    void read_calib_imgs();

    //! Find corners in images
    void find_corners();
    void initialize_cams();
    void initialize_cams_ref();

    void write_BA_data();
    void write_BA_data_ref();
    void run_BA();
    void run_BA_ref();

    double run_BA_pinhole(baProblem &ba_problem, string ba_file, Size img_size, vector<int> const_points);
    double run_BA_refractive(baProblem_plane &ba_problem, string ba_file, Size img_size, vector<int> const_points);

    void write_calib_results();
    void write_calib_results_ref();
    void load_calib_results();

    void write_calib_results_matlab();
    void write_calib_results_matlab_ref();

    void grid_view();

    // Functions to access calibration data
    //! \return Number of cameras in the data set
    int num_cams() { return num_cams_; }
    //! \return Number of images per camera in the data set
    int num_imgs() { return num_imgs_; }
    //! \return Size in number of internal corners of grid used    
    Size grid_size() { return grid_size_; }
    //! \return Names of cameras (names of folders in which images lie if images are separate or names of the multipage tiff files)
    vector<string> cam_names() { return cam_names_; }
    refocusing_data refocusing_params() { return refocusing_params_; }

 private:

    void calc_space_warp_factor();
    void get_grid_size_pix();

    string path_;
    string ba_file_;
    string result_dir_;
    string result_file_;
    
    int num_cams_;
    int num_imgs_;
    int center_cam_id_;
    int origin_image_id_;
    
    Size grid_size_;
    Size img_size_;
    
    double grid_size_phys_;
    double grid_size_pix_;
    double pix_per_phys_;
        
    vector<string> cam_names_;
    vector< vector<Mat> > calib_imgs_;
    vector< vector< vector<Point3f> > > all_pattern_points_;
    vector< vector< vector<Point2f> > > all_corner_points_;
    vector< vector< vector<Point2f> > > all_corner_points_raw_;
    vector<Mat> cameraMats_;
    vector<Mat> dist_coeffs_;
    vector< vector<Mat> > rvecs_;
    vector< vector<Mat> > tvecs_;
    
    vector<Mat> rVecs_;
    vector<Mat> tVecs_;
    vector<Mat> K_mats_;
    vector<Mat> dist_mats_;
    
    vector<int> const_points_;

    refocusing_data refocusing_params_;

    baProblem ba_problem_;
    baProblem_plane ba_problem_ref_;
    double total_reproj_error_;
    double total_error_;
    double avg_reproj_error_;
    double avg_error_;

    int refractive_;
    float geom[5];
    
    
    // Option flags
    int solveForDistortion; // TODO: NOT IMPLEMENTED
    int squareGrid; // TODO: NOT IMPLEMENTED
    int saveCornerImgs; // TODO: NOT IMPLEMENTED
    int show_corners_flag;
    int run_calib_flag;
    int results_just_saved_flag;
    int load_results_flag;
    int dummy_mode_;
    int refractive_;
    int mtiff_; // 0 = folders with tif images, 1 = multipage tif files

    // Settings
    int pinhole_max_iterations;
    int refractive_max_iterations;
    int skip_frames_;
    float init_f_value_;

    // Order control variables
    int cam_names_read_;
    int images_read_;
    int corners_found_;
    int cams_initialized_;
    
};
    
#endif
