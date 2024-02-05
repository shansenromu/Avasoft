import subprocess

def run_root_script(script_path,output_file):
    try:
        # Run ROOT script

        with open(output_file, 'w') as output:
            subprocess.run(['/home/hansens/root_intstall/bin/root', '-b' , '-q', script_path], check=True,stdout=output)
    except subprocess.CalledProcessError as e:
        print(f"Error running ROOT script: {e}")

def run_shell_command(command):
    try:
        # Run shell command
        subprocess.run(command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
       print(f"Error running command: {e}")

if __name__ == "__main__":
    # Run ROOT script
    root_script_path = "readInInten.C"
    run_root_script(root_script_path,"/home/hansens/Data5up2/readInInten_output.txt")

    root_script_path = "FitIntensitySpec.C"
    run_root_script(root_script_path,"/home/hansens/Data5up2/FitResults.txt")
 
    root_script_path = "MeanPeakGuess.C"
    run_root_script(root_script_path,"/home/hansens/Data5up2/GuessPeak.txt")

    shell_command = "grep -E '^[0-9]+\s+[0-9]+\.[0-9]+\s+[0-9]+\.[0-9]+$' GuessPeak.txt"
    run_shell_command(shell_command)
  
