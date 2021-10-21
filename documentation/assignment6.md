@page assignment6 A6: Final project

@m_footernavigation
@tableofcontents

Project Theme: "It's what's inside that counts"
===============================================

Project proposal
================
@m_span{m-label m-primary m-flat} 1 pt @m_endspan

In `reports/final_project_proposal/report.html` of your git repository, create a short project proposal according to the template there. This should include:

* A proposed motivational image. This doesn't have to be a photo you took. It can be an image you find online, but make sure to properly credit the source. You could also include multiple images if that better conveys your vision for your final project.
* A few sentences describing how the image conforms with this year's theme.
* A list of features relevant to your image that you'd like to implement along with a tentative point breakdown which sums up to 16 points (see details of suggested point breakdowns below).
* If there are some "nice to have" stretch goal features, include these as a separate list.
* A short plan for how you will work together/divide the work.

This should be a short proposal. Roughly 1 page.


Submitting your proposal
------------------------

You are submitting with a partner. Use the People tab in Canvas to join a group with your partner. You will then submit a URL to one of your repo's commits just like in previous assignments (see the \ref submitting-on-canvas section of \ref getting-started guide). You are welcome to both duplicate the report in your own repos if you like, but you will submit the URL for just one of them as a team.


Final project
=============
Features & point breakdown
--------------------------
@m_span{m-label m-primary m-flat} 16 pts @m_endspan

In your renderer you are required to implement a minimum set of features that will be showcased in your report for grading. Features/Functionality are split into 3 categories: simple, moderate and advanced. Your task is to submit in your report any combination of the following types of features that sums up to 16 points. _The only restriction is that you must implement at least 1 advanced feature_.

* Simple features can get up to a maximum of 1 points each (e.g. depth of field) 
* Moderate features can get up to a maximum of 4 points each (e.g. environment map emitter with importance sampling, texturing and bump mapping, or anisotropic microfacet BRDF) 
* Advanced features can get up to a maximum of 8 points (e.g. photon mapping, participating media, or subsurface scattering) 

The above point breakdowns are just a rough guide. Its possible to e.g. implement a very sophisticated version of a simple algorithm and this would be worth more than a bare-bones version of the same method. Correctness and sufficient _validation_ will highly impact the final grade. It is your job to convince us that your implementation is correct. For instance, an untested and buggy implementation of an advanced feature such as the beam radiance estimate may end up getting you less points than a very well tested, and sophisticated depth-of-field implementation. Each graded feature you point out in your report will be graded for completeness and correctness. In your report you should have:

* Feature-specific renderings showcasing a scene that emphasizes the feature's effect. 
* Technical details of the implementation.
* _Validation tests_ performed and their results.
* Code coordinates for the bulk of the feature's implementation. 

We will not be giving out a list of approved features for each category, we leave this up to you. Graded features which you don't end up using in your final image must still include the above details for grading.


@m_class{m-note m-danger}

@par Warning
    The details below of the format of the presentation, time limits, etc. will likely change as the plans for the rendering competition materialize. 

Presentation & Report
---------------------
@m_span{m-label m-primary m-flat} 3 pts @m_endspan

The rendering competition will replace the final examination period on November 19th from 8am-11am.

We will quickly introduce the jury and then start with the presentations in random order. Each student will have up to (TBD) minutes of presentation followed by up to (TBD) minutes of questions by the jury. After the presentations the jury will deliberate and choose the winners. We will end the competition with the award ceremony!

Neatness and completeness of your oral presentation and your report will be graded.

Grading
-------

Only code that you implemented will be graded. Code that was graded in previous assignments will not be considered for grading in the final project. Code that was not written during the class or not by the student(s) submitting the project will not be considered for grading and _must_ be properly documented/credited.

What to submit
--------------

You will be submitting your report/presentation containing the info described above according to the instructions in the \ref submitting-on-canvas section of \ref getting-started guide.

Here are some tips for structuring your `report.html`:

* Include your motivational image and discuss its relation to the theme. Point out the interesting phenomena in the image.

* Structure the document in sections where in each section you describe a new feature of your renderer.
    
  * For every selected (graded) feature, include any feature-specific images you generated (if applicable). The image(s) must convince us that the functionality is implemented correctly and completely. You can follow the validation paradigm we used in the other assignments.

  * Include descriptions of encountered problems, a list of external libraries that you used, and other comments for each feature.

  * If you added any functionality that is not being submitted for grading, include a short section describing these.

* End with a section showcasing your final submitted image.

Past Year's Winning Entries
===========================

For inspiration, have a look at the past year's winning entries in the rendering competition conducted as a part of the image synthesis class taught at ETH below.

![The above submission by Dario Seyb was the winning entry in the 2019 rendering competition. The theme was 'The Outer Limits'.](images/dario_seyb_2019.png)

![The above submission Shruti Agarwal was the winning entry in the 2018 rendering competition. The theme was 'Something Seems a Little Off'.](images/shruti_agarwal_2018.png)

![The above submission was the winning entry in the 2015 rendering competition, submitted by Tizian Zeltner. The theme was 'Made in Switzerland'.](images/tizian_zeltner_2015.png)

![The above submission was the winning entry in the 2015 computer graphics rendering competition, submitted by Simon Kallweit. The theme was 'Classical Elements'.](images/simon_kallweit_2015.png)

![The above submission was the winning entry in the 2014 rendering competition, submitted by Benedikt Bitterli. The theme was 'That belongs in a museum'.](images/benedikt_bitterli_2014.png)

@m_footernavigation
