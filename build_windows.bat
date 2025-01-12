@echo off

if "%1"=="clean" (
    rmdir /S /Q out
) else if "%1" == "build" (
    
    if exist "out" (
        cd out
        cmake --build . --config Debug -- /m:1
    ) else (
        mkdir out
        cd out
        cmake -G"Visual Studio 17 2022" ${COMMON_CMAKE_CONFIG_PARAMS} ../
        cmake --build . --config Debug -- /m:1
    )
    cd ..

) else if "%1" == "run" (

    if exist "out" (
        cd out
        cmake --build . --config Debug -- /m:1
    ) else (
        mkdir out
        cd out
        cmake -G"Visual Studio 17 2022" ${COMMON_CMAKE_CONFIG_PARAMS} ../
        cmake --build . --config Debug -- /m:1
    )

    cd ..
    .\bin\2d_sidescroller.exe
)




