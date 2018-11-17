rm -r hallF*
rm -r hallS*
rm -r obj

mkdir obj

mkdir hallFrame
mkdir hallFrame3x3F
mkdir hallFrame3x3FF
mkdir hallFrame3x3O
mkdir hallFrame3x3OF
mkdir hallFrameSIMD
mkdir hallFrameSIMD_M
mkdir hallSigma
mkdir hallSigmaSIMD
mkdir hallFrame3x3O_pipe
mkdir hallFrame3x3F_pipe
mkdir hallFrame3x3F_bin
mkdir hallFrame3x3O_bin
mkdir Save
make clean && make
