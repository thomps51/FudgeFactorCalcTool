histDir=output/hists
pdfsDir=output/pdfs
[[ ! -d "$histDir" ]] && echo "default output directory $histDir not made, making..." && mkdir -p $histDir
[[ ! -d "$pdfsDir" ]] && echo "default output directory $pdfsDir not made, making..." && mkdir -p $pdfsDir
[[ ! -d libs ]] && mkdir libs
setupATLAS
localSetupROOT     # definitely working with 6.04.14-x86_64-slc6-gcc49-opt



