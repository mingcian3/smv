@echo off
:: generate terrain with &GEOM
set option=-geom

set dem2fds=..\intel_win_64\dem2fds_win_64.exe
set dem2fds=dem2fds
::set "terrain=%userprofile%\terrain"
set "terrain=%userprofile%\Google drive\documents\terrain"

%dem2fds% %option%       -dir "%terrain%\campfire" campfire1.in 
%dem2fds% %option% -show -dir "%terrain%\campfire" campfire2.in 
