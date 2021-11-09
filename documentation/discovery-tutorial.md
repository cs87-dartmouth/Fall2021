# How to use DARTS on the discovery cluster


## Logging in

If you are outside the dartmouth network, make sure you first connect to the [VPN](https://services.dartmouth.edu/TDClient/1806/Portal/KB/ArticleDet?ID=66806).

Then SSH into the login node:

```
ssh username@discovery7.dartmouth.edu
```

## Clone your git repo

First you must load a more recent git module into your environment. Run the following command:

```
module load git/2.0.4
```

You'll then want to clone your GitHub repo somewhere within your home directory on discovery. To get this to work you'll need to either set up your personal access tokens or ssh keys on GitHub. Once you do, clone your repo.

## Set up modules (do this every time you log in)

Now that you have your repo, navigate to its directory, and run the `setup-discovery.sh` script. This will load the appropriate version of modules you'll need on discovery and link the correct version of the GNU compiler.

## Set up CPM_SOURCE_CACHE to speed up CMake configures (do this once)

Go to your home directory and create a cache for CPM:

```
mkdir -p $HOME/.cache/CPM
```

Open up your `.bash_profile` in a text editor (e.g. using the command `nanoand $HOME/.bash_profile`) and export the `CPM_SOURCE_CACHE` variable to make the end of the file look like this:

```
# User specific environment and startup programs
export CPM_SOURCE_CACHE=$HOME/.cache/CPM
```

Save the file (in `nano` hit `Ctrl-O`, then `Ctrl-X`).

## Configure DARTS

Make sure you are in your root DARTS directory and run the `configure-discovery.sh` script to configure darts.

You should see something like this:

```
./configure-discovery.sh 
-- The CXX compiler identification is GNU 9.3.1
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /opt/rh/devtoolset-9/root/usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Setting build type to 'Release' as none was specified.
-- CPM: adding package PackageProject.cmake@1.6.0 (v1.6.0 at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/packageproject.cmake/d10abef59d67d4a5c303763ad9a3331bf4c1ee01)
-- CPM: adding package nlohmann_json@3.9.1 (3.9.1 at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/nlohmann_json/da58edb2f54861722a77eb11ffdeecbeb52e1388)
-- CPM: adding package filesystem@0 (5e0eb2a6160201de38d076c085641742ee86a8f6 at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/filesystem/b39e61be0349f6bf8972ab3b87cadb320dc25b51)
-- CPM: adding package pcg32@0 (70099eadb86d3999c38cf69d2c55f8adc1f7fe34 at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/pcg32/aab2de9185202aba859550ca88570771b7bd6186)
-- CPM: adding package stb@0 (a0a939058c579ddefd4c5671b046f29d12aeae01 at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/stb/1532208e8ba0d1cd421ca8a37564c1e9d5510f34)
-- Module support is disabled.
-- Version: 8.0.1
-- Build type: Release
-- CXX_STANDARD: 11
-- Performing Test has_std_11_flag
-- Performing Test has_std_11_flag - Success
-- Performing Test has_std_0x_flag
-- Performing Test has_std_0x_flag - Success
-- Performing Test SUPPORTS_USER_DEFINED_LITERALS
-- Performing Test SUPPORTS_USER_DEFINED_LITERALS - Success
-- Performing Test FMT_HAS_VARIANT
-- Performing Test FMT_HAS_VARIANT - Success
-- Required features: cxx_variadic_templates
-- Looking for strtod_l
-- Looking for strtod_l - found
-- CPM: adding package fmt@0 (02ad5e11da5b2702cc1c9a8fdf750486beee08fe at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/fmt/6ec6854fc3bb3b708ccc3655098e3cfd3c75ab58)
-- CPM: adding package linalg@0 (a3e87da35e32b781a4b6c01cdd5efbe7ae51c737 at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/linalg/23311a3ee1fed93350ea9775549e3cd19440fa0d)
-- CPM: adding package cli11@1.9.1 (1.9.1 at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/cli11/104aafd130e26975700131221e1b1bd8c6e814df)
-- CPM: adding package tinyobjloader@0 (94d2f7fe1f7742818dbcd0917d11679d055a33de at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/tinyobjloader/b8df2eec7967726c83e859b84b0f2ad30c5f516d)
-- CPM: adding package tinyexr@1.0.0 (v1.0.0 at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/tinyexr/e225d422e497cbe157d01838a907a2b1a0e21bb9)
-- Build spdlog: 1.9.2
-- Looking for C++ include pthread.h
-- Looking for C++ include pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE  
-- Build type: Release
-- Generating install
-- CPM: adding package spdlog@1.9.2 (v1.9.2 at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/spdlog/cced5c35ec43466bb67ce80a52ac7dff03377357)
-- Check if compiler accepts -pthread
-- Check if compiler accepts -pthread - yes
-- The C compiler identification is GNU 9.3.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /opt/rh/devtoolset-9/root/usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- CPM: adding package m.css@0 (51a02af8132b3ba1f632fa7928f46a411e6d844e at /dartfs-hpc/rc/home/s/f00274s/.cache/CPM/m.css/3ea9065d1ec3371990723aa130d92784aeea4de0)
-- Configuring done
-- Generating done
-- Build files have been written to: /dartfs-hpc/rc/home/s/f00274s/darts-solutions/build
```

## Compiling DARTS

Now compile DARTS. To avoid overloading the login node, we will do this using the `srun` command which will run this on a compute node:

```
srun --account=cs_comp --mem=16G build-discovery.sh
```

The `--account` and `--mem` parameters are important.

You should get something like this:

```
$ srun --account=cs_comp --mem=16G build-discovery.sh
Consolidate compiler generated dependencies of target fmt
[  4%] Built target fmt
Consolidate compiler generated dependencies of target spdlog
[ 13%] Built target spdlog
[ 16%] Building CXX object CMakeFiles/darts_lib.dir/src/common.cpp.o
[ 17%] Building CXX object CMakeFiles/darts_lib.dir/src/image.cpp.o
[ 18%] Building CXX object CMakeFiles/darts_lib.dir/src/progress.cpp.o
[ 20%] Building CXX object CMakeFiles/darts_lib.dir/src/integrators/ao.cpp.o
[ 21%] Building CXX object CMakeFiles/darts_lib.dir/src/integrators/integrator.cpp.o
[ 21%] Building CXX object CMakeFiles/darts_lib.dir/src/integrators/normals.cpp.o
[ 22%] Building CXX object CMakeFiles/darts_lib.dir/src/integrators/path_tracer_simple.cpp.o
[ 24%] Building CXX object CMakeFiles/darts_lib.dir/src/integrators/path_tracer_mats.cpp.o
[ 27%] Building CXX object CMakeFiles/darts_lib.dir/src/integrators/path_tracer_mis.cpp.o
[ 28%] Building CXX object CMakeFiles/darts_lib.dir/src/integrators/path_tracer_mixture.cpp.o
[ 28%] Building CXX object CMakeFiles/darts_lib.dir/src/integrators/path_tracer_nee.cpp.o
[ 29%] Building CXX object CMakeFiles/darts_lib.dir/src/integrators/vol_path_tracer_mis.cpp.o
[ 31%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/blend.cpp.o
[ 32%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/blinn_phong.cpp.o
[ 33%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/diffuse_light.cpp.o
[ 35%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/henyey_greenstein.cpp.o
[ 36%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/isotropic.cpp.o
[ 37%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/normal_map.cpp.o
[ 39%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/phong.cpp.o
[ 40%] Building CXX object CMakeFiles/darts_lib.dir/src/media/homogeneous.cpp.o
[ 41%] Building CXX object CMakeFiles/darts_lib.dir/src/media/vacuum.cpp.o
[ 43%] Building CXX object CMakeFiles/darts_lib.dir/src/samplers/cmj.cpp.o
[ 44%] Building CXX object CMakeFiles/darts_lib.dir/src/samplers/independent.cpp.o
[ 45%] Building CXX object CMakeFiles/darts_lib.dir/src/samplers/ld.cpp.o
[ 48%] Building CXX object CMakeFiles/darts_lib.dir/src/samplers/oa.cpp.o
[ 48%] Building CXX object CMakeFiles/darts_lib.dir/src/samplers/stratified.cpp.o
[ 50%] Building CXX object CMakeFiles/darts_lib.dir/src/surfaces/envmap.cpp.o
[ 51%] Building CXX object CMakeFiles/darts_lib.dir/src/surfaces/point_light.cpp.o
[ 52%] Building CXX object CMakeFiles/darts_lib.dir/src/textures/checker.cpp.o
[ 54%] Building CXX object CMakeFiles/darts_lib.dir/src/textures/constant.cpp.o
[ 55%] Building CXX object CMakeFiles/darts_lib.dir/src/textures/marble.cpp.o
[ 56%] Building CXX object CMakeFiles/darts_lib.dir/src/textures/texture.cpp.o
[ 58%] Building CXX object CMakeFiles/darts_lib.dir/src/perlin.cpp.o
[ 60%] Building CXX object CMakeFiles/darts_lib.dir/src/tests/surface_sample_test.cpp.o
[ 60%] Building CXX object CMakeFiles/darts_lib.dir/src/tests/material_sample_test.cpp.o
[ 63%] Building CXX object CMakeFiles/darts_lib.dir/src/tests/test.cpp.o
[ 63%] Building CXX object CMakeFiles/darts_lib.dir/src/surfaces/mesh.cpp.o
[ 64%] Building CXX object CMakeFiles/darts_lib.dir/src/surfaces/triangle.cpp.o
[ 66%] Building CXX object CMakeFiles/darts_lib.dir/src/surfaces/bbh.cpp.o
[ 67%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/dielectric.cpp.o
[ 70%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/lambertian.cpp.o
[ 70%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/material.cpp.o
[ 71%] Building CXX object CMakeFiles/darts_lib.dir/src/materials/metal.cpp.o
[ 74%] Building CXX object CMakeFiles/darts_lib.dir/src/surfaces/quad.cpp.o
[ 75%] Building CXX object CMakeFiles/darts_lib.dir/src/surfaces/sphere.cpp.o
[ 75%] Building CXX object CMakeFiles/darts_lib.dir/src/surfaces/surface.cpp.o
[ 78%] Building CXX object CMakeFiles/darts_lib.dir/src/surfaces/surface_group.cpp.o
[ 78%] Building CXX object CMakeFiles/darts_lib.dir/src/parser.cpp.o
[ 79%] Building CXX object CMakeFiles/darts_lib.dir/src/scene.cpp.o
[ 81%] Building CXX object CMakeFiles/darts_lib.dir/src/example_scenes.cpp.o
[ 81%] Built target darts_lib
[ 83%] Building CXX object CMakeFiles/darts.dir/src/darts.cpp.o
[ 85%] Building CXX object CMakeFiles/darts_tutorial0.dir/src/darts_tutorial0.cpp.o
[ 86%] Building CXX object CMakeFiles/test_tri_intersection.dir/src/test_tri_intersection.cpp.o
[ 86%] Building CXX object CMakeFiles/img_compare.dir/src/img_compare.cpp.o
[ 90%] Building CXX object CMakeFiles/img_avg.dir/src/img_avg.cpp.o
[ 90%] Building CXX object CMakeFiles/point_gen.dir/src/point_gen.cpp.o
[ 90%] Building CXX object CMakeFiles/darts_tutorial1.dir/src/darts_tutorial1.cpp.o
[ 91%] Linking CXX executable point_gen
[ 91%] Built target point_gen
[ 93%] Linking CXX executable darts_tutorial0
[ 94%] Linking CXX executable test_tri_intersection
[ 94%] Built target darts_tutorial0
[ 94%] Built target test_tri_intersection
[ 95%] Linking CXX executable darts_tutorial1
[ 95%] Built target darts_tutorial1
[ 97%] Linking CXX executable img_avg
[ 98%] Linking CXX executable img_compare
[ 98%] Built target img_avg
[ 98%] Built target img_compare
[100%] Linking CXX executable darts
[100%] Built target darts
```

If this fails, modify the `build-discovery.sh` script by removing the `--parallel` flag on the `cmake` command, and try again.

## Rendering images on the cluster

To render a scene, you'll need to create a slurm script which instructs the job scheduler how to run your program. We provide a template slurm script to run a job array `test-slurm-job.sh`. Open it up to see how it is structured. There are several special comments that give parameters to the scheduler. The most important line is at the very bottom where you list what executable will be run.

This test script will render the `jensen_box_mats.json` scene on 32 separate nodes, each saving its result as an `.exr` file in the `results` directory (note, you will need to create this output directory otherwise your jobs will not write out anything). You could run this job using the command:

```
sbatch ./test-slurm-job.sh
```

If all goes well, you should see something like this:

```
Submitted batch job 695045
```

## Checking on the status of your render jobs

To check the progress of your render jobs you can run the command:

```
squeue
```

For the above test job I got the following result:

```
             JOBID PARTITION     NAME     USER ST       TIME  NODES NODELIST(REASON)
    695045_[30-32]  standard darts_re  mynetid PD       0:00      1 (AssocGrpCpuLimit)
          695045_0  standard darts_re  mynetid  R       0:18      1 m08
          695045_1  standard darts_re  mynetid  R       0:18      1 m08
          695045_2  standard darts_re  mynetid  R       0:18      1 m16
          695045_3  standard darts_re  mynetid  R       0:18      1 m16
          695045_4  standard darts_re  mynetid  R       0:18      1 k04
          695045_5  standard darts_re  mynetid  R       0:18      1 k05
          695045_6  standard darts_re  mynetid  R       0:18      1 k06
          695045_7  standard darts_re  mynetid  R       0:18      1 q03
          695045_8  standard darts_re  mynetid  R       0:18      1 q03
          695045_9  standard darts_re  mynetid  R       0:18      1 q03
         695045_10  standard darts_re  mynetid  R       0:18      1 q03
         695045_11  standard darts_re  mynetid  R       0:18      1 q03
         695045_12  standard darts_re  mynetid  R       0:18      1 q03
         695045_13  standard darts_re  mynetid  R       0:18      1 q03
         695045_14  standard darts_re  mynetid  R       0:18      1 q04
         695045_15  standard darts_re  mynetid  R       0:18      1 q04
         695045_16  standard darts_re  mynetid  R       0:18      1 q04
         695045_17  standard darts_re  mynetid  R       0:18      1 q04
         695045_18  standard darts_re  mynetid  R       0:18      1 q04
         695045_19  standard darts_re  mynetid  R       0:18      1 q05
         695045_20  standard darts_re  mynetid  R       0:18      1 q05
         695045_21  standard darts_re  mynetid  R       0:18      1 q05
         695045_22  standard darts_re  mynetid  R       0:18      1 q05
         695045_23  standard darts_re  mynetid  R       0:18      1 q05
         695045_24  standard darts_re  mynetid  R       0:18      1 q05
         695045_25  standard darts_re  mynetid  R       0:18      1 q05
         695045_26  standard darts_re  mynetid  R       0:18      1 q07
         695045_27  standard darts_re  mynetid  R       0:18      1 q07
         695045_28  standard darts_re  mynetid  R       0:18      1 q07
         695045_29  standard darts_re  mynetid  R       0:18      1 q07
```


## Specifying the random seed in DARTS

To be able to average images rendered on different nodes, you'll want to specify the random seed for DARTS from the commandline. We did this by adding a `--seed` commandline argument to our implementation, and we used this value to specify the seed in the Scene's Sampler. When running your job you should then specify a different seed for each node which you can do by passing this command-line option to darts on the last line of your slurm script  `--seed $SLURM_ARRAY_TASK_ID`.


