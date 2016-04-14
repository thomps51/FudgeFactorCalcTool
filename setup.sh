[[ ! -d "libs" ]] && echo "library directory libs/ not made, making..." && mkdir -p libs
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
lsetup root     # definitely working with 6.04.14-x86_64-slc6-gcc49-opt



