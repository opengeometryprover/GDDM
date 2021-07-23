package ogp;

public class OGPConf {

    private final static String MAJOR_VERSION = "0";
    private final static String MINOR_VERSION = "0";
    private final static String VERSION = MAJOR_VERSION + "." + MINOR_VERSION;

    private final static String GLOBAL_CONF = "/usr/local/share/ogp/ogp.conf";
    private final static String USER_CONF = "$HOME/.ogprc";

    public OGPConf() {
    }

    public String getVersion() {
	return this.VERSION;
    }
    
}
