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
		
		String pathName = "C:\\Program Files\\mosquitto\\";
		String[] command = new String[10];
		
		
		if (System.getProperty("os.name") == "Linux") {
			pathName = "/etc/mosquitto";
			command[1] = "/etc/mosquitto/mosquitto_sub";
			
		}else {
			pathName = "C:\\Program Files\\mosquitto\\";
			command[1] = "C:\\Program Files\\mosquitto\\mosquitto_sub.exe";
		}
		

		ProcessBuilder builder = new ProcessBuilder(command); // create mosquitto_sub process listener
		builder = builder.directory(new File(pathName));

		try {
			System.out.println("builder in directory: " + builder.directory());
			System.out.println("Attempting to start ");
			Process process = builder.start();
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

		} catch (IOException e) {
			System.out.println("wow, it caught an exception");
			System.out.println(e);
			
		}
		
	}
	static void publish(String time) {
		System.out.println("printing time: vvv");
		System.out.println(time);

		Scanner stdin = new Scanner(System.in);

		String pathName = "C:\\Program Files\\mosquitto\\";
		String[] command = { "C:\\Program Files\\mosquitto\\mosquitto_pub.exe", "-h", "192.168.0.13", "-p", "1883",
				"-t", "time", "-m", time};

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


