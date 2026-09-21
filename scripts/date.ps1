$fileObjectList = [System.Collections.ArrayList]@()
$files = Get-ChildItem ..\textos

foreach ($file in $files) {
	$dateString = Get-Content -Path ..\textos\$file -TotalCount 1
	$date = [datetime]::parseexact($dateString, 'dd/MM/yyyy | HH:mm', $null)
	$fileObject = [PSCustomObject]@{File = $file; Date = $date }
	$null = $fileObjectList.Add($fileObject)
}

$fileObjectList = $fileObjectList | Sort-Object -Property Date -Descending

Write-Host '<ul>'
foreach ($fileObject in $fileObjectList) {
	$file = $fileObject.File
	$date = $fileObject.Date.ToString("dd/MM/yyyy")
	Write-Host '<li>'
	Write-Host "$date <a href=""textos/$file"">$file</a>"
	Write-Host '</li>'
}
Write-Host '</ul>'