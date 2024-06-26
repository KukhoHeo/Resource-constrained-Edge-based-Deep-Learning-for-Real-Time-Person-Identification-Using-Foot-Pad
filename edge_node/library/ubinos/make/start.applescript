#! /usr/bin/env /usr/bin/osascript

--osascript start.applescript "`pwd`" ...

on run argv

  tell application "Terminal"
    set AppleScript's text item delimiters to " "

    set cwd to first item of argv
    set l to rest of argv

    if application "Terminal" is running then
      do script "cd " & cwd & " && " & (l as text)
    else
      do script "cd " & cwd & " && " & (l as text) in window 1
    end if
    activate
  end tell

end run

