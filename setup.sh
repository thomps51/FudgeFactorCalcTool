histDir=output/hists
pdfsDir=output/pdfs
skimDir=output/skimmedNtups
DIRS=(libs output/hists output/pdfs output/skimmedNtups)
for DIR in ${DIRS[@]}; do
  [[ ! -d "$DIR" ]] && echo "default output directory $DIR not made, making..." && mkdir -p $DIR
done
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
lsetup root     # definitely working with 6.04.14-x86_64-slc6-gcc49-opt



