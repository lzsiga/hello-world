public class Outer {
    static String sfield= "SO"; /* static field */
    String ifield= "IO";        /* instance-field */

    public class PublicInner {
        /* cannot have `static` methods */
        void inst_hello() {
            System.out.printf (
                  "hello from PublicInner.inst_hello, sfield='%s' ifield='%s'\n"
                + "  this=%s\n  Outer.this=%s\n"
                , Outer.sfield         /* or simply `sfield` */
                , Outer.this.ifield    /* or simply `ifield` */
                , this
                , Outer.this);
        }
    }

    public static class PublicStaticInner {
        static final String sPSI= "PSI";
        static void hello () {
            System.out.printf ("hello from PublicStaticInner.hello, sfield='%s', sPI='%s'\n",
                Outer.sfield, /* or simply `sfield` */
                Outer.PrivateInner.sPI);
        }
        void inst_hello() {
            System.out.printf ("hello from PublicStaticInner.inst_hello, sfield='%s'\n"
                , Outer.sfield);
        }
    }

    private class PrivateInner {
        /* cannot have `static` methods */
        /* cannot have `static` fields, unless final */
        static final String sPI= "PI"; /* ez is csak azert lehet itt, mert `final` */
        void inst_hello() {
            System.out.printf ("hello from PrivateInner.inst_hello, sfield='%s' ifield='%s'\n",
                Outer.sfield,        /* or simply `sfield` */
                Outer.this.ifield);  /* or simply `ifield` */
        }
    }

    private static class PrivateStaticInner {
        static void hello () {
            System.out.printf ("hello from PrivateStaticInner.hello, sfield='%s', sPSI='%s'\n",
                Outer.sfield, /* or simply `sfield` */
                Outer.PublicStaticInner.sPSI);
        }
        void inst_hello() {
            System.out.printf ("hello from PrivateStaticInner.inst_hello, sfield='%s'\n",
                Outer.sfield);       /* or simply `sfield` */
        }
    }

    public static void main (String args[]) {
        Outer o= new Outer();
        System.out.printf ("main starts, o=%s\n", o);
        PublicStaticInner.hello ();
        PrivateStaticInner.hello ();

        Outer.PublicInner  pi= o.new PublicInner();
        Outer.PrivateInner ri= o.new PrivateInner();

        pi.inst_hello();
        ri.inst_hello();

        Outer.PublicStaticInner  psi= new Outer.PublicStaticInner();
        Outer.PrivateStaticInner rsi= new Outer.PrivateStaticInner();

        psi.inst_hello();
        rsi.inst_hello();

        System.out.printf ("main exits\n");
    }
}
