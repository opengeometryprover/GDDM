package ogptest;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.concurrent.TimeUnit;
import java.util.Scanner;

public class OGPTestProver {

    public OGPTestProver(OGPTestArgs arguments) {
	try {
	    // File
	    System.out.println("FILE: " + arguments.getFile());
	    System.out.println();
	    File theFile = new File(arguments.getFile());
	    Scanner theFileScanner = new Scanner(theFile);
	    while (theFileScanner.hasNextLine()) {
		System.out.println(theFileScanner.nextLine());
	    }
	    System.out.println();
	    theFileScanner.close();
	    // Timeout
	    System.out.print("TIMEOUT: " + arguments.getTimeout() + "...");
	    TimeUnit.SECONDS.sleep(arguments.getTimeout());
	    System.out.println(" Done!!!");
	} catch (FileNotFoundException e) {
	    errorMsg(999, e.toString());
	} catch (InterruptedException e) {
	    errorMsg(101, e.toString());
	}
    }

    private static void errorMsg(int error, String msg) {
	System.err.print("[OGPTest ERROR " + error + "] (OGPTestProver) ");
	switch (error) {
	case 101:
	    System.err.println("Program interrupted.");
	    System.err.println(msg);
	    break;
	case 999:
	    System.err.println("Something is really wront :-|");
	    System.err.println(msg);
	    break;
	}
    }

}
