DOXYFILE = '@CMAKE_CURRENT_BINARY_DIR@/Doxyfile-mcss'

MAIN_PROJECT_URL = 'https://cs87-dartmouth.github.io/Fall2021'

STYLESHEETS = [
    'https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i%7CSource+Code+Pro:400,400i,600',
    '../css/m-dark.css',
    '../css/m-documentation.css'
]

FAVICON = '@CMAKE_CURRENT_LIST_DIR@/images/favicon.png'

HTML_HEADER = "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js\"></script>" \
              "<script src=\"jquery.event.move.js\"></script>" \
              "<script src=\"jquery.twentytwenty.js\"></script>" \
              "<link href=\"twentytwenty.css\" rel=\"stylesheet\" type=\"text/css\" />" \
              "<script>" \
              "$(window).load(function(){$(\".twentytwenty-container\").twentytwenty({default_offset_pct: 0.5, move_slider_on_hover: true });});" \
              "</script>"


LINKS_NAVBAR1 = [
    ('Schedule', 'schedule', []),
    (None, 'assignments', [
        (None, 'assignment0'),
        (None, 'assignment1'),
        (None, 'assignment2'),
        (None, 'assignment3'),
        (None, 'assignment4'),
        (None, 'assignment5'),
        (None, 'assignment6')
    ])
]

LINKS_NAVBAR2 = [
    ("Darts", 'darts-overview', [
        ("<a href=\"darts-overview.html\">Darts</a>", ),
        (None, 'getting-started'),
        ("<a href=\"https://github.com/cs87-dartmouth/Fall2021\">GitHub</a>", ),
        (None, 'modules'),
        (None, 'annotated'),
        (None, 'files')
    ]),
    (None, 'credits', []),
]

FINE_PRINT = '<p>CS 87/287 Rendering Algorithms. Created with <a href="https://doxygen.org/">Doxygen</a> {doxygen_version} and <a href="https://mcss.mosra.cz/">m.css</a>.<br />See <a href="credits.html">credits page</a> for additional acknowledgements.</p>'

# CLASS_INDEX_EXPAND_INNER = True


EXTRA_FILES = [
    '@CMAKE_CURRENT_LIST_DIR@/m-theme-dark.css',
    '../css/m-grid.css',
    '../css/m-components.css',
    '../css/m-layout.css',
    '../css/pygments-dark.css',
    '../css/pygments-console.css',
    '@DOXYGEN_PROJECT_ROOT@/reports/resources/jquery.event.move.js',
    '@DOXYGEN_PROJECT_ROOT@/reports/resources/jquery.twentytwenty.js',
    '@DOXYGEN_PROJECT_ROOT@/reports/resources/twentytwenty.css',
    '@CMAKE_CURRENT_LIST_DIR@/images/loewenfeld.png',
    '@CMAKE_CURRENT_LIST_DIR@/images/shoe.png',
    '@CMAKE_CURRENT_LIST_DIR@/images/classroom.png',
    '@CMAKE_CURRENT_LIST_DIR@/images/nefertiti.png',
    '@CMAKE_CURRENT_LIST_DIR@/images/jensen_box.png',
    '@CMAKE_CURRENT_LIST_DIR@/images/RTOneWeekend.jpg',
    '@CMAKE_CURRENT_LIST_DIR@/images/RTNextWeek.jpg',
    '@CMAKE_CURRENT_LIST_DIR@/images/RTRestOfYourLife.jpg',
    '@CMAKE_CURRENT_LIST_DIR@/images/pbrt-bookcover.png',

    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment0/cornellbox.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment0/cornell_grayscale-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment0/gradient-ref.png',

    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/01_camera_ray_image-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/01_manual_ray_image-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/01_xformed_camera_ray_image-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/01_recursive_raytracing-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/01_xformed_camera_sphere_image-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/example_scene0-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/example_scene1-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/example_scene2-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/example_scene3-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/example_scene1-1spp.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/example_scene2-1spp.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/example_scene3-1spp.png',

    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/01_plane-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/02_balls-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/03_refl-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/04_refr-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/05_quad-light-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment1/06_jensen_box-ref.png',

    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment2/simple-geometry-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment2/bunny-dragons-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment2/buddha-box-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment2/sponza-buddha-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment2/ajax-white-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment2/nefertiti-white-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment2/loewenfeld-white-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment2/loewenfeld-in-box-ref.png',

    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/constant-cornell-box-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/checker-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/marble-128spp.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/earth-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/textured-box-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/teapot-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/blend-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/fresnel_blend-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/ajax-textured-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/nefertiti-textured-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment3/loewenfeld-textured-ref.png',

    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/ajax-normals-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/blinn_phong-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/phong-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/ajax-ao-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/odyssey_mats-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/veach_mats-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/lambertian-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/lambertian-sampled-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/lambertian-rotated-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/lambertian-rotated-sampled-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/phong-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/phong-sampled-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/phong-rotated-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/phong-rotated-sampled-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/blinn_phong-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/blinn_phong-sampled-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/blinn_phong-rotated-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment4/blinn_phong-rotated-sampled-ref.png',

    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/odyssey_nee-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/odyssey_mis-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/odyssey_triangle_mats-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/odyssey_triangle_nee-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/odyssey_triangle_mis-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/veach_nee-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/veach_mis-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_small-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_medium-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_large-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_small_mats-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_small_nee-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_small_mis-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_medium_mats-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_medium_nee-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_medium_mis-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_large_mats-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_large_nee-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere_light_large_mis-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/sphere-sampled-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/quad-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/quad-sampled-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/triangle-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/triangle-sampled-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/group-pdf-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/group-sampled-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/jensen_box_mats-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/jensen_box_nee-ref.png',
    '@DOXYGEN_PROJECT_ROOT@/scenes/assignment5/jensen_box_mis-ref.png',
]
