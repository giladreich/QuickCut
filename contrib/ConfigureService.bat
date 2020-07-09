@echo off

sc config QuickCutService start=delayed-auto
sc failure QuickCutService actions=restart/60000ms/restart/60000ms/restart/60000ms// reset=86400
