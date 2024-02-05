import subprocess

def run_root_script(script_path,output_file):
    try:
        # Run ROOT script

        with open(output_file, 'w') as output:
            subprocess.run(['/home/hansens/root_intstall/bin/root', '-b' , '-q', script_path], check=True,stdout=output)
    except subprocess.CalledProcessError as e:
        print(f"Error running ROOT script: {e}")

def run_root_script2(script_path, output_file, arg1, arg2, arg3):
    try:
        # Run ROOT script and capture output to a file
        with open(output_file, 'w') as output:
            subprocess.run(['/home/hansens/root_intstall/bin/root', '-b', '-q', script_path, str(arg1), str(arg2), str(arg3)], check=True, stdout=output)
    except subprocess.CalledProcessError as e:
        print(f"Error running ROOT script: {e}")
        
def run_shell_command(command):
    try:
        # Run shell command
        subprocess.run(command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
       print(f"Error running command: {e}")

if __name__ == "__main__":
 

    root_script_path = "FitIntensitySpec.C"
    run_root_script2(root_script_path,"/home/hansens/Data5up2/FitResults.txt", 50, 70.8955, 0.526548)
 
    root_script_path = "ProcessFitResults.C"
    run_root_script(root_script_path,"/home/hansens/Data5up2/ProcessFitResults.txt")



