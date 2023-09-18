import time
import subprocess
import pyautogui




def step_motor_up(stepNum):
    print('Step motor up')
    pyautogui.click(x=21,y=1008) # click on windows start, this prevents cosmos from minimizing on a double click
    pyautogui.click(x=460,y=1004) #click on cosmos process from the app bar
    time.sleep(0.1)
    pyautogui.click(x=136,y=160) #click on Motor 1
    time.sleep(0.1)
    # Double click the text box
    pyautogui.click(x=252, y=139) #click distance text box
    time.sleep(0.1)
    pyautogui.click(x=252, y=139) #click  distance text box
    time.sleep(0.1)
    pyautogui.press('del') # delete current number
    time.sleep(0.1)
    steps_to_type = str(stepNum)
    pyautogui.typewrite(steps_to_type, interval=0.1) #input step distance
    pyautogui.click(x=213,y=194) #click on negative to start motor going up in wavelength
    time.sleep(int( float(stepNum)/2000. )+1 )
    
def step_motor_down(stepNum):
    print('Step motor down')
    pyautogui.click(x=21,y=1008) # click on windows start, this prevents cosmos from minimizing on a double click
    pyautogui.click(x=460,y=1004) #click on cosmos process from the app bar
    time.sleep(0.1)
    pyautogui.click(x=136,y=160) #click on Motor 1
    # Click and hold at the initial position
    pyautogui.click(x=252, y=139) #click distance text box
    time.sleep(0.1)
    pyautogui.click(x=252, y=139) #click  distance text box
    time.sleep(0.1)
    pyautogui.press('del')
    time.sleep(0.1)
    steps_to_type = str(stepNum+100)
    pyautogui.typewrite(steps_to_type, interval=0.1) #input distance
    pyautogui.click(x=309,y=194) #click on positive to start motor going down in wavelength
    time.sleep(int( float(stepNum)/2000. )+1 )
    #  There is slack in the system (i.e. the belt) attempt to
    # remove error cause by the slack by always approaching from the side going up
    pyautogui.click(x=136,y=160) #click on Motor 1
    # Click and hold at the initial position
    pyautogui.click(x=252, y=139) #click distance text box
    time.sleep(0.1)
    pyautogui.click(x=252, y=139) #click  distance text box
    time.sleep(0.1)
    pyautogui.press('del')
    time.sleep(0.1)
    steps_to_type = str(100)
    pyautogui.typewrite(steps_to_type, interval=0.1) #input distance
    pyautogui.click(x=213,y=194) #click on negative to start motor going up in wavelength
    time.sleep(int( 25./2000. )+1 )

    
# Function to control the stepper motor using COMOS Velmex
def control_stepper_motor():
    # Your code to interface with COMOS Velmex and control the stepper motor
    cosmos_process = subprocess.Popen("C:\\Users\\Amilia\\Downloads\\COSMOS.exe")
    # Wait for COSMOS to start
    time.sleep(15)  # Adjust as needed

    #pyautogui.hotkey('win','up')

    #Run command that takes a number of steps
    # 100,000 step negative takes dial from 1.9 to 45.0
    # 0.000 431 dial/step
    # 2320.18561485 steps/dial
    # from 0-900 on dial 2,088,167
    # choosing 2320 from now on
    cosmos_process.terminate()

# Function to automate AVASoft and save spectra
def save_spectrum_with_avasoft(filePrefix, fileNum):
    print('saving spectrum')
    # Start AVASoft using subprocess
    #avasoft_process = subprocess.Popen("C:\Users\Amilia\Avantes\AvaSoft8\avasoft8.exe")

    ##If Avasof is out of date
    ## Code to skip through update dialog box
    ## Simulate pressing the Tab key
    #pyautogui.press('tab')
    #time.sleep(1) 
    ## Simulate pressing the Right Arrow key
    #pyautogui.press('right')
    #time.sleep(1) 
    ## Simulate pressing the Enter key
    #pyautogui.press('enter')
    #time.sleep(1) 


    # Your code to interact with AVASoft UI using pyautogui
    # Screen shots used to find the coordinates of cursor neccary to export to ASCII

    # Take Spectrum and Export it
    pyautogui.click(x=21,y=1008) # click on windows start, this prevents Avasoft from minimizing on a double click
    pyautogui.click(x=412,y=1004) #click on Avasoft in the process app bar
    #pyautogui.click(x=77,y=303)  #take dark
    pyautogui.click(x=285,y=176) #clicking on home
    #pyautogui.click(x=496,y=237)  #clicking on absolute intensity 
    pyautogui.click(x=497,y=206) #clicking on spectrum minus dark
    pyautogui.click(x=27,y=74)  #start 
    pyautogui.click(x=327,y=177)  #File 
    pyautogui.click(x=452,y=212)  #Export
    pyautogui.click(x=500,y=250)  #ASCII
    pyautogui.press('enter')
    time.sleep(1)
    # Write file name
    filename_to_type = filePrefix +'_' + str(fileNum)
    time.sleep(1)
    pyautogui.typewrite(filename_to_type, interval=0.1)
    pyautogui.press('enter')
    time.sleep(3)

    
    # Close AVASoft
    #avasoft_process.terminate()
    print('spectrum saved')


# Function to automate AVASoft and save intensity
def save_intensity_with_avasoft(filePrefix, fileNum):
    print('saving intensity')
    
    # Code to interact with AVASoft UI using pyautogui
    # Screen shots used to find the coordinates of cursor neccary to export to ASCII

    # Take Intensity Spectrum and Export it
    pyautogui.click(x=21,y=1008) # click on windows start, this prevents Avasoft from minimizing on a double click
    pyautogui.click(x=412,y=1004) #click on Avasoft in the process app bar 
    #pyautogui.click(x=77,y=303)  #take dark
    time.sleep(1)
    # click on the absolute intensity
    pyautogui.click(x=285,y=176) #clicking on home
    pyautogui.click(x=496,y=237)  #clicking on absolute intensity 
    #pyautogui.click(x=497,y=206) #clicking on spectrum minus dark

    
    pyautogui.click(x=30,y=73)  #start
    # click on the absolute intensity graph
    pyautogui.press('enter')   
    pyautogui.click(x=332,y=177)  #File
    pyautogui.click(x=452,y=213)  #Export
    pyautogui.click(x=478,y=254)  #ASCII
    pyautogui.press('enter')
    time.sleep(1)
    # Write file name
    filename_to_type = filePrefix +'_' + str(fileNum)
    time.sleep(1)
    pyautogui.typewrite(filename_to_type, interval=0.1)
    pyautogui.press('enter')
    time.sleep(3)

    
    # Close AVASoft
    #avasoft_process.terminate()
    print('intensity saved')


# Main script

# NewGrating we want to start from 470 and end near top of range.
# OldGrating we want to start from ??? and end near top of range.

# Control the stepper motor
#control_stepper_motor()
#Ask user input values of Axis1 and convert to steps and home to 100
#steps
print('Running Script')

# # Go from AAA to BBB by SSS saving along the up swing then reset to AAA
#dial=AAA
#i=0
#while dial<=BBB:
#    interval_1 = SSS
#    step = int(2320*interval_1)
#    #Save spectrum using AVASoft
#    save_spectrum_with_avasoft('Spec_Name_dail_'+str(dial),i)
#    save_intensity_with_avasoft('Inten_Name_dial_'+str(dial),i)
#    dial = dial + interval_1
#    step_motor_up(step)
#    i=i+1
#step_motor_down(int((2320)*(dial-100)))

# # Go from AAA to BBB by XXX times then reset
#dial=AAA
#for i in range(XXX):
#    interval_2 = BBB-AAA
#    step = int(2320*interval_2)
#    #Save spectrum using AVASoft
#    save_spectrum_with_avasoft('Spec_Name_low_dial_'+str(dial),i)
#    save_intensity_with_avasoft('Spec_Name_low_dial_'+str(dial),i)
#    step_motor_up(step)
#    save_spectrum_with_avasoft('Spec_Name_high_dial_'+str(dial),i)
#    save_intensity_with_avasoft('Spec_Name_high_dial_'+str(dial),i)
#    step_motor_down(step)



# # Take data every TTT at BBB XXX times
#dial=AAA
#waitTime1 = TTT
#curTime1 = TTT
#interval_2 = BBB-AAA
#step = int(2320*interval_2)
#step_motor_up(step)
#while curTime1<XXX*waitTime:
#    #get the current time to save in the spec file
#    curTime2 = ???
#    save_spectrum_with_avasoft('Spec_Name_dial_'+str(dial)+'_time_'+str(curTime2),i)
#    save_intensity_with_avasoft('Spec_Name_dial_'+str(dial)+'_time_'+str(curTime2),i)
#    time.sleep(waitTime)
#    curTime1 = curTime1+waitTime
#step_motor_down(step)


# Go from 50 to 850 by 10 saving along the up swing then reset to 50
start_dial=50
dial = start_dial
i=0
while dial<=850:
    interval_1 = 10
    step = int(2320*interval_1)
    #Save spectrum using AVASoft
    save_spectrum_with_avasoft('Spec_360nmDG_3160umS_dail_'+str(dial),i)
    save_intensity_with_avasoft('Inten_360nmDG_3160umS_dial_'+str(dial),i)
    dial = dial + interval_1
    step_motor_up(step)
    i=i+1
step_motor_down(int((2320)*(dial-100)))



print('Macro End')

