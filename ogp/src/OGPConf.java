package ogp;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class OGPConf {

    private final static String MAJOR_VERSION = "0";
    private final static String MINOR_VERSION = "0";
    private final static String VERSION = MAJOR_VERSION + "." + MINOR_VERSION;

    private final static String CONF_FILE = "/usr/local/share/ogp/ogp.conf";

    public OGPConf() {
	readConfFile(CONF_FILE);
	readConfFile(System.getenv("HOME")
		     + System.getProperty("file.separator")
		     + ".ogprc");
    }

    public String getVersion() {
	return this.VERSION;
    }

    private static void readConfFile(String file) {
	try {
	    File confFile = new File(file);
	    if (confFile.canRead() && confFile.isFile()) {
		Scanner confFileScanner = new Scanner(confFile);
		while (confFileScanner.hasNextLine()) {
		    String line = confFileScanner.nextLine();
		    System.out.println(line);
		}
		confFileScanner.close();
	    }
	} catch (FileNotFoundException e) {
	    errorMsg(99, e.toString());
	}
    }

    private static void errorMsg(int error, String str) {
	System.err.print("[ERROR " + error + "] Conf : ");
	switch (error) {
	case 99:
	    System.err.println("Something is really wrong :-|");
	    System.err.println(str);
	    break;
	}
	System.exit(error);
    }

}
