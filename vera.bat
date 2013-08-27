@echo off

dir /s /b src\*.cpp src\*.h src\*.inl | vera++ --summary --root "C:\Program Files (x86)\vera++\lib\vera++" -o vera_report