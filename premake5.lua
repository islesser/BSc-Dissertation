workspace "Tidal"
    architecture "x64"
    configurations
    {
        "Debug", 
        "Release"
    }

out = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Tidal"
    location "Tidal"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. out .. "/%{prj.name}")
    objdir ("bin-int/" .. out .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**",
    }
    sysincludedirs { "%{prj.name}/src" }

    includedirs
    {
        "vendor/glfw/include",
        "vendor/glad/include",
        "vendor/glm",
        "vendor/spdlog/include",
        "%{prj.name}/src",
        "/usr/local/Cellar/opencv/4.1.2/include"
    }

    libdirs {
        "vendor/glfw/lib",
        "/usr/local/Cellar/opencv/4.1.2/lib"
    }

    filter "system:windows"
        systemversion "latest"
        
        links {
            "glfw3",
            "opengl32",
        }
        
        defines
        {
            "PT_WIN"
        }

    filter "action:vs*"  -- for Visual Studio actions
        pchheader "PCHeader.h"
        pchsource "%{prj.name}/src/PCHeader.cpp"

    filter "action:not vs*"  -- for everything else
        pchheader "%{prj.name}/src/PCHeader.h"

    
    filter "system:macosx"
        links {
            "glfw3",
            "OpenGL.framework",
            "Cocoa.framework",
            "IOKit.framework",
            "CoreVideo.framework"
        }

        defines
        {
            "PT_MAC"
        }
        linkoptions
        {
            "-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_photo -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_face -lopencv_fuzzy -lopencv_img_hash -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ml -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_xobjdetect -lopencv_imgcodecs -lopencv_objdetect -lopencv_xphoto -lopencv_imgproc -lopencv_core"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        buildoptions {}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"