package ogptest;

public class OGPTestConf {

    private final static String MAJOR_VERSION = "0";
    private final static String MINOR_VERSION = "5";
    
    private final static String VERSION = MAJOR_VERSION + "." + MINOR_VERSION;

    public OGPTestConf() {
	// Nothing to do
    }

    public String getVersion() {
	return this.VERSION;
    }

}
