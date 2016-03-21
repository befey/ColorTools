# DIR=$(cd "$(dirname "$(readlink -f "$0")")" && pwd)

BASEDIR="$( dirname "$0" )"
cd "$BASEDIR"

# Remove all old output
rm ColorTools.zxp
rm -R ./unzipped\ extension\ panel/*

./ZXPSignCmd -sign ../com.gosafeguard.ColorTools ColorTools.zxp MyCert.p12 abc123 -tsa https://timestamp.geotrust.com/tsa

mkdir ./unzipped\ extension\ panel/com.gosafeguard.ColorTools
tar -xf ColorTools.zxp -C ./unzipped\ extension\ panel/com.gosafeguard.ColorTools