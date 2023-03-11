// Generated from d:\dev\kapa\repos\tarracsh\src\infrastructure\db\query\grammar\KapaQuery.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class KapaQueryLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, NOT_EQUAL=6, EQUAL=7, REGEX=8, 
		STARS_WITH=9, END_WITH=10, AND=11, OR=12, Identifier=13, WS=14, EscapedString=15;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "T__1", "T__2", "T__3", "T__4", "NOT_EQUAL", "EQUAL", "REGEX", 
			"STARS_WITH", "END_WITH", "AND", "OR", "Identifier", "WS", "EscapedString", 
			"Escape"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'schema'", "'list'", "'where'", "'('", "')'", "'!='", "'='", "'*^*'", 
			"'^*'", "'*^'", "'and'", "'or'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, "NOT_EQUAL", "EQUAL", "REGEX", "STARS_WITH", 
			"END_WITH", "AND", "OR", "Identifier", "WS", "EscapedString"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public KapaQueryLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "KapaQuery.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\21\u0084\b\1\4\2"+
		"\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4"+
		"\13\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\3\2\3"+
		"\2\3\2\3\2\3\2\3\2\3\2\3\3\3\3\3\3\3\3\3\3\3\4\3\4\3\4\3\4\3\4\3\4\3\5"+
		"\3\5\3\6\3\6\3\7\3\7\3\7\3\b\3\b\3\t\3\t\3\t\3\t\3\n\3\n\3\n\3\13\3\13"+
		"\3\13\3\f\3\f\3\f\3\f\3\r\3\r\3\r\3\16\3\16\7\16R\n\16\f\16\16\16U\13"+
		"\16\3\17\6\17X\n\17\r\17\16\17Y\3\17\3\17\3\20\3\20\3\20\3\20\3\20\7\20"+
		"c\n\20\f\20\16\20f\13\20\3\20\3\20\3\20\3\20\3\20\3\20\7\20n\n\20\f\20"+
		"\16\20q\13\20\3\20\3\20\3\20\3\20\7\20w\n\20\f\20\16\20z\13\20\3\20\5"+
		"\20}\n\20\3\21\3\21\3\21\3\21\5\21\u0083\n\21\3x\2\22\3\3\5\4\7\5\t\6"+
		"\13\7\r\b\17\t\21\n\23\13\25\f\27\r\31\16\33\17\35\20\37\21!\2\3\2\b\5"+
		"\2C\\aac|\6\2\62;C\\aac|\5\2\13\f\17\17\"\"\3\2$$\3\2))\4\2\u201f\u201f"+
		"\u2035\u2035\2\u008f\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2"+
		"\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3"+
		"\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3\2\2"+
		"\2\3#\3\2\2\2\5*\3\2\2\2\7/\3\2\2\2\t\65\3\2\2\2\13\67\3\2\2\2\r9\3\2"+
		"\2\2\17<\3\2\2\2\21>\3\2\2\2\23B\3\2\2\2\25E\3\2\2\2\27H\3\2\2\2\31L\3"+
		"\2\2\2\33O\3\2\2\2\35W\3\2\2\2\37|\3\2\2\2!\u0082\3\2\2\2#$\7u\2\2$%\7"+
		"e\2\2%&\7j\2\2&\'\7g\2\2\'(\7o\2\2()\7c\2\2)\4\3\2\2\2*+\7n\2\2+,\7k\2"+
		"\2,-\7u\2\2-.\7v\2\2.\6\3\2\2\2/\60\7y\2\2\60\61\7j\2\2\61\62\7g\2\2\62"+
		"\63\7t\2\2\63\64\7g\2\2\64\b\3\2\2\2\65\66\7*\2\2\66\n\3\2\2\2\678\7+"+
		"\2\28\f\3\2\2\29:\7#\2\2:;\7?\2\2;\16\3\2\2\2<=\7?\2\2=\20\3\2\2\2>?\7"+
		",\2\2?@\7`\2\2@A\7,\2\2A\22\3\2\2\2BC\7`\2\2CD\7,\2\2D\24\3\2\2\2EF\7"+
		",\2\2FG\7`\2\2G\26\3\2\2\2HI\7c\2\2IJ\7p\2\2JK\7f\2\2K\30\3\2\2\2LM\7"+
		"q\2\2MN\7t\2\2N\32\3\2\2\2OS\t\2\2\2PR\t\3\2\2QP\3\2\2\2RU\3\2\2\2SQ\3"+
		"\2\2\2ST\3\2\2\2T\34\3\2\2\2US\3\2\2\2VX\t\4\2\2WV\3\2\2\2XY\3\2\2\2Y"+
		"W\3\2\2\2YZ\3\2\2\2Z[\3\2\2\2[\\\b\17\2\2\\\36\3\2\2\2]d\7$\2\2^c\5!\21"+
		"\2_`\7$\2\2`c\7$\2\2ac\n\5\2\2b^\3\2\2\2b_\3\2\2\2ba\3\2\2\2cf\3\2\2\2"+
		"db\3\2\2\2de\3\2\2\2eg\3\2\2\2fd\3\2\2\2g}\7$\2\2ho\7)\2\2in\5!\21\2j"+
		"k\7)\2\2kn\7)\2\2ln\n\6\2\2mi\3\2\2\2mj\3\2\2\2ml\3\2\2\2nq\3\2\2\2om"+
		"\3\2\2\2op\3\2\2\2pr\3\2\2\2qo\3\2\2\2r}\7)\2\2sx\7\u201e\2\2tw\5!\21"+
		"\2uw\13\2\2\2vt\3\2\2\2vu\3\2\2\2wz\3\2\2\2xy\3\2\2\2xv\3\2\2\2y{\3\2"+
		"\2\2zx\3\2\2\2{}\t\7\2\2|]\3\2\2\2|h\3\2\2\2|s\3\2\2\2} \3\2\2\2~\177"+
		"\7b\2\2\177\u0083\7)\2\2\u0080\u0081\7b\2\2\u0081\u0083\7$\2\2\u0082~"+
		"\3\2\2\2\u0082\u0080\3\2\2\2\u0083\"\3\2\2\2\r\2SYbdmovx|\u0082\3\b\2"+
		"\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}