# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

add_library(libcommontool STATIC  thread.cpp
            	message_queue/handler.cpp
            	message_queue/message_queue.cpp
            	egl_core/egl_core.cpp
            	egl_core/egl_share_context.cpp

            	opensl_media/opensl_es_context.cpp

            	opengl_media/movie_frame.cpp
                opengl_media/texture/texture_frame.cpp
                opengl_media/texture/gpu_texture_frame.cpp
                opengl_media/texture/yuv_texture_frame.cpp
                opengl_media/texture_copier/texture_frame_copier.cpp
                opengl_media/texture_copier/gpu_texture_frame_copier.cpp
                opengl_media/texture_copier/yuv_texture_frame_copier.cpp
                opengl_media/render/video_gl_surface_render.cpp )