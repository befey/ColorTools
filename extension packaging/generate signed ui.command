# DIR=$(cd "$(dirname "$(readlink -f "$0")")" && pwd)

BASEDIR="$( dirname "$0" )"
cd "$BASEDIR"

# Remove all old output
rm SafeguardTools.zxp
rm -R ./unzipped\ extension\ panel/*

./ZXPSignCmd -sign ../com.gosafeguard.SafeguardTools SafeguardTools.zxp MyCert.p12 abc123 -tsa http://sha256timestamp.ws.symantec.com/sha256/timestamp

mkdir ./unzipped\ extension\ panel/com.gosafeguard.SafeguardTools
tar -xf SafeguardTools.zxp -C ./unzipped\ extension\ panel/com.gosafeguard.SafeguardTools
