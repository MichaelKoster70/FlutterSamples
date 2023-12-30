# ----------------------------------------------------------------------------
# <copyright company="Michael Koster">
#   Copyright (c) Michael Koster. All rights reserved.
#   Licensed under the MIT License.
# </copyright>
# ----------------------------------------------------------------------------
# Create Self Signed x.509 Cert for APPX Development
# ----------------------------------------------------------------------------

$cert = New-SelfSignedCertificate -Type Custom -Subject "CN=Michael Koster" -KeyUsage DigitalSignature -FriendlyName "APPX Dev Cert Michael Koster" -CertStoreLocation "Cert:\CurrentUser\My" -TextExtension @("2.5.29.37={text}1.3.6.1.5.5.7.3.3", "2.5.29.19={text}")

$credentials = Get-Credential -User "PFX File" -Message "Enter password for PFX file"
$thumbPrint = $cert.Thumbprint
Export-PfxCertificate -cert "Cert:\CurrentUser\My\$thumbPrint" -FilePath DevCert.pfx -Password $credentials.Password
Export-Certificate -cert "Cert:\CurrentUser\My\$thumbPrint" -FilePath DevCert.cer
