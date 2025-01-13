# PowerShell Script to Archive Current Directory and Exclude Specific Items Recursively

# Set the path to the 7-Zip executable (update if not in PATH)
$sevenZipPath = "7z"

# Define the archive directory and exclusion list
$archiveDir = ".\archive"

# Generate the archive file name based on the current date and time
$timestamp = Get-Date -Format "yyyy-MM-dd.HHmm"
$archiveName = "$timestamp.7z"

# Create the archive directory if it doesn't exist
if (!(Test-Path -Path $archiveDir)) {
    New-Item -ItemType Directory -Path $archiveDir
}

# Create the archive including only .c and .h files
& $sevenZipPath a "$archiveDir\$archiveName" -i@listfile 

# Check for success
if ($?) {
    Write-Host "Archive created successfully: $archiveDir\$archiveName"
} else {
    Write-Host "Error occurred during archiving"
}
