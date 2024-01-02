# ----------------------------------------------------------------------------
# <copyright company="Michael Koster">
#   Copyright (c) Michael Koster. All rights reserved.
#   Licensed under the MIT License.
# </copyright>
# ----------------------------------------------------------------------------
# Script to generate the Package Resource Index (PRI) config file
# ----------------------------------------------------------------------------

# use Windows 11 SDK
$windowsSdk = "${env:ProgramFiles(x86)}\Windows Kits\10\bin\10.0.22000.0\x64"
$makepri = Join-Path $windowsSdk -ChildPath "makepri.exe" 

$scriptDir = Split-Path -Parent $PSCommandPath

#generate the config file in the script foder
&$makePri createconfig /ConfigXml $scriptDir\priconfig.xml /Default en-US /Overwrite /Platform