@echo off
echo Downloading SDL3...

set SDL3_VERSION=3.2.10
set SDL3_URL=https://github.com/libsdl-org/SDL/releases/download/release-%SDL3_VERSION%/SDL3-%SDL3_VERSION%-win64-x64.zip

powershell -Command "Invoke-WebRequest -Uri '%SDL3_URL%' -OutFile 'C:\Users\Administrator\Desktop\NovaStella-1\SDL3.zip'"
powershell -Command "Expand-Archive -Path 'C:\Users\Administrator\Desktop\NovaStella-1\SDL3.zip' -DestinationPath 'C:\Users\Administrator\Desktop\NovaStella-1\deps' -Force"

echo Downloading SDL3_image...
set SDL3_IMAGE_URL=https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.0/SDL3_image-3.2.0-win64-x64.zip

powershell -Command "Invoke-WebRequest -Uri '%SDL3_IMAGE_URL%' -OutFile 'C:\Users\Administrator\Desktop\NovaStella-1\SDL3_image.zip'"
powershell -Command "Expand-Archive -Path 'C:\Users\Administrator\Desktop\NovaStella-1\SDL3_image.zip' -DestinationPath 'C:\Users\Administrator\Desktop\NovaStella-1\deps' -Force"

echo Downloading fmt...
set FMT_URL=https://github.com/fmtlib/fmt/releases/download/12.1.3/fmt-12.1.3-win64.zip

powershell -Command "Invoke-WebRequest -Uri '%FMT_URL%' -OutFile 'C:\Users\Administrator\Desktop\NovaStella-1\fmt.zip'"
powershell -Command "Expand-Archive -Path 'C:\Users\Administrator\Desktop\NovaStella-1\fmt.zip' -DestinationPath 'C:\Users\Administrator\Desktop\NovaStella-1\deps' -Force"

echo Downloading minimp3...
set MINIMP3_URL=https://github.com/llib/minimp3/releases/download/2024.01.09/minimp3-2024.01.09-win64.zip

powershell -Command "Invoke-WebRequest -Uri '%MINIMP3_URL%' -OutFile 'C:\Users\Administrator\Desktop\NovaStella-1\minimp3.zip'"
powershell -Command "Expand-Archive -Path 'C:\Users\Administrator\Desktop\NovaStella-1\minimp3.zip' -DestinationPath 'C:\Users\Administrator\Desktop\NovaStella-1\deps' -Force"

echo Dependencies downloaded and extracted!
