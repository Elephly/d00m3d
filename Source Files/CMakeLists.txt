# Specify project files: header files and source files
set(HDR_DIR "${PROJECT_SOURCE_DIR}/Header Files")
set(RES_DIR "${PROJECT_SOURCE_DIR}/Source Files/Resources")

set(HDRS
  "${HDR_DIR}/asteroid.h"
  "${HDR_DIR}/black_hole.h"
  "${HDR_DIR}/black_hole_projectile.h"
  "${HDR_DIR}/black_hole_cannon.h"
  "${HDR_DIR}/camera.h"
  "${HDR_DIR}/defines.h"
  "${HDR_DIR}/enemy_spacecraft.h"
  "${HDR_DIR}/entity.h"
  "${HDR_DIR}/input_manager.h"
  "${HDR_DIR}/item.h"
  "${HDR_DIR}/laser.h"
  "${HDR_DIR}/laser_cannon.h"
  "${HDR_DIR}/mesh_factory.h"
  "${HDR_DIR}/ogre_app_exception.h"
  "${HDR_DIR}/ogre_application.h"
  "${HDR_DIR}/particle_effect.h"
  "${HDR_DIR}/particle_engine.h"
  "${HDR_DIR}/physics_engine.h"
  "${HDR_DIR}/physics_entity.h"
  "${HDR_DIR}/plasma_ball.h"
  "${HDR_DIR}/plasma_cannon.h"
  "${HDR_DIR}/player_spacecraft.h"
  "${HDR_DIR}/resource.h"
  "${HDR_DIR}/spacecraft.h"
  "${HDR_DIR}/star.h"
  "${HDR_DIR}/system_component.h"
  "${HDR_DIR}/TextRenderer.h"
  "${HDR_DIR}/weapon.h"
  "${HDR_DIR}/world.h"
)

set(SRCS
  asteroid.cpp
  black_hole.cpp
  black_hole_cannon.cpp
  black_hole_projectile.cpp
  camera.cpp
  enemy_spacecraft.cpp
  entity.cpp
  input_manager.cpp
  item.cpp
  laser.cpp
  laser_cannon.cpp
  main.cpp
  mesh_factory.cpp
  ogre_application.cpp
  particle_effect.cpp
  particle_engine.cpp
  physics_engine.cpp
  physics_entity.cpp
  plasma_ball.cpp
  plasma_cannon.cpp
  player_spacecraft.cpp
  resource.cpp
  spacecraft.cpp
  star.cpp
  system_component.cpp
  TextRenderer.cpp
  weapon.cpp
  world.cpp
  "${RES_DIR}/.ShaderDefs.material"
  "${RES_DIR}/asteroid.material"
  "${RES_DIR}/berlin_sans32.fontdef"
  "${RES_DIR}/black_hole.material"
  "${RES_DIR}/enemy_spacecraft.material"
  "${RES_DIR}/explosion.material"
  "${RES_DIR}/explosionMaterialFp.glsl"
  "${RES_DIR}/explosionMaterialGp.glsl"
  "${RES_DIR}/explosionMaterialVp.glsl"
  "${RES_DIR}/FlameParticle.material"
  "${RES_DIR}/FlameParticleMaterialFp.glsl"
  "${RES_DIR}/FlameParticleMaterialGp.glsl"
  "${RES_DIR}/FlameParticleMaterialVp.glsl"
  "${RES_DIR}/item.material"
  "${RES_DIR}/MaterialFp.glsl"
  "${RES_DIR}/MaterialVp.glsl"
  "${RES_DIR}/MyFont.fontdef"
  "${RES_DIR}/player_spacecraft.material"
  "${RES_DIR}/red_laser.material"
  "${RES_DIR}/spacecraft.material"
  "${RES_DIR}/Skybox.material"
  "${RES_DIR}/SkyboxFp.glsl"
  "${RES_DIR}/SkyboxVp.glsl"
  "${RES_DIR}/Spline.material"
  "${RES_DIR}/SplineMaterialFp.glsl"
  "${RES_DIR}/SplineMaterialGp.glsl"
  "${RES_DIR}/SplineMaterialVp.glsl"
  "${RES_DIR}/star.material"
  "${RES_DIR}/TextureMaterialFp.glsl"
  "${RES_DIR}/TextureMaterialVp.glsl"
  "${RES_DIR}/weapon.material"
)

# The rules here are specific to Windows Systems
if(WIN32)
    # Get Ogre directory from the environment variable
    set(OGRE_HOME $ENV{OGRE_HOME})

    # Set up Ogre include directories
    include_directories(
		"${HDR_DIR}"
        "${OGRE_HOME}/include"
        "${OGRE_HOME}/include/OIS"
        "${OGRE_HOME}/include/OGRE"
        "${OGRE_HOME}/boost"
    )

    # Set up Ogre library directories
    link_directories(
        "${OGRE_HOME}/lib/"
        "${OGRE_HOME}/boost/lib"
    )

    # Applications using Ogre need to be run from inside the Ogre
    # directory. This cmake command needs to come before add_executable
    set(EXECUTABLE_OUTPUT_PATH "${OGRE_HOME}/bin/")

    # Add path name
    configure_file(path_config.h.in path_config.h)

    # Add executable based on the source files
    add_executable(${PROJECT_NAME} ${HDRS} ${SRCS})

    # Set up names of Ogre libraries
    target_link_libraries(${PROJECT_NAME}
        optimized "OgreMain.lib"
        optimized "OIS.lib"
        optimized "OgreOverlay.lib"
        debug "OgreMain_d.lib"
        debug "OIS_d.lib"
        debug "OgreOverlay_d.lib"
    )

    # Avoid ZERO_CHECK target 
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
    
    # Custom post build commands
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory \"%\(FullPath\).dir\\..\\..\\..\\Source Files\\Resources\" \"$(OutDir)\\Resources\"
    )
endif(WIN32)
