package mqttListener;

import java.util.Scanner;
import java.io.*;
import java.awt.*;
import java.time.*;
import java.text.SimpleDateFormat;
import java.util.Date;

public class mqttListener {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("in main --firsssssssst");
		System.out.println(System.getProperty("os.name"));
		listener();
		

	}

	public static void listener() {
		System.out.println("starting");

		Scanner stdin = new Scanner(System.in);
		
		String pathName = "";
		String[] command = new String[7];
		
		String OS = System.getProperty("os.name");
		System.out.println(OS);
		
			pathName = "/bin";
			command[0] = "/bin/mosquitto_sub";
			command[1] = "-t";
			command[2] = "time";
			command[3] = "-h";
			command[4] = "192.168.0.25";
			command[5] = "-p";
			command[6] = "1883";	
	

		ProcessBuilder builder = new ProcessBuilder(command); // create mosquitto_sub process listener
		System.out.println(pathName);
		builder = builder.directory(new File(pathName));

		try {
			System.out.println("builder in directory: " + builder.directory());
			System.out.println("Attempting to start ");
			System.out.println("mosquitto sub?" + command[0]);
			Process process = builder.start();
			System.out.println("Started buidler successfully");
			InputStream stream = process.getInputStream();
			BufferedReader reader = new BufferedReader(new InputStreamReader(stream));

			String line = null;
			while ((line = reader.readLine()) != null) {
				line = reader.readLine();
				System.out.println(line);

				SimpleDateFormat formatter = new SimpleDateFormat("HH");
				Date date = new Date();
				
				System.out.println(formatter.format(date));
				System.out.println(line);
			
				if (line.charAt(0)== 't') {
					System.out.println("inside IF");
					System.out.println(LocalDateTime.now());
					
					publish(formatter.format(date));
					
				}
			}

		} catch (NullPointerException e) {
			System.out.println("wow, it caught an exception");
			System.out.println(e);
			
		} catch (IOException e) {
			System.out.println("there's a captured IOException");
		}
		
	}
	static void publish(String time) {
		System.out.println("printing time: vvv");
		System.out.println(time);

		Scanner stdin = new Scanner(System.in);

		String pathName = "/bin";
		String[] command = { "/bin/mosquitto_pub", "-h", "192.168.0.25","-t", "time","-m", time};

		ProcessBuilder builder = new ProcessBuilder(command); // create mosquitto_pub process listener
		builder = builder.directory(new File(pathName));

		try {
		
			System.out.println("Attempting to publish ");
			Process process = builder.start();
//			InputStream stream = process.getInputStream();
//			BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
//
//			String line = null;
//			while ((line = reader.readLine()) != null) {
//				System.out.println(line);
//
//				SimpleDateFormat formatter = new SimpleDateFormat("HH:mm:ss");
//				Date date = new Date();
//
//				System.out.println(formatter.format(date));
//				if (line == "time") {
//					System.out.println(LocalDateTime.now());
//					publish(LocalDateTime.now());
//					
//				}
//			}

		} catch (IOException e) {
			System.out.println("wow, it caught an exception");
			System.out.println(e);
			
		}
	}

		
	}


