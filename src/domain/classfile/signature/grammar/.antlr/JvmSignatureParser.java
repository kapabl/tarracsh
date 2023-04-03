// Generated from d:\dev\kapa\repos\tarracsh\src\domain\classfile\signature\grammar\JvmSignature.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class JvmSignatureParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, T__16=17, 
		T__17=18, T__18=19, T__19=20, T__20=21, T__21=22, T__22=23, Identifier=24;
	public static final int
		RULE_all = 0, RULE_javaTypeSignature = 1, RULE_baseType = 2, RULE_referenceTypeSignature = 3, 
		RULE_classTypeSignature = 4, RULE_packageSpecifier = 5, RULE_simpleClassTypeSignature = 6, 
		RULE_typeArguments = 7, RULE_typeArgument = 8, RULE_wildcardIndicator = 9, 
		RULE_classTypeSignatureSuffix = 10, RULE_typeVariableSignature = 11, RULE_arrayTypeSignature = 12, 
		RULE_classSignature = 13, RULE_typeParameters = 14, RULE_typeParameter = 15, 
		RULE_classBound = 16, RULE_superclassSignature = 17, RULE_superInterfaceSignature = 18, 
		RULE_interfaceBound = 19, RULE_methodSignature = 20, RULE_result = 21, 
		RULE_throwsSignature = 22, RULE_voidDescriptor = 23, RULE_fieldSignature = 24;
	private static String[] makeRuleNames() {
		return new String[] {
			"all", "javaTypeSignature", "baseType", "referenceTypeSignature", "classTypeSignature", 
			"packageSpecifier", "simpleClassTypeSignature", "typeArguments", "typeArgument", 
			"wildcardIndicator", "classTypeSignatureSuffix", "typeVariableSignature", 
			"arrayTypeSignature", "classSignature", "typeParameters", "typeParameter", 
			"classBound", "superclassSignature", "superInterfaceSignature", "interfaceBound", 
			"methodSignature", "result", "throwsSignature", "voidDescriptor", "fieldSignature"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'B'", "'C'", "'D'", "'F'", "'I'", "'J'", "'S'", "'Z'", "'L'", 
			"';'", "'/'", "'<'", "'>'", "'*'", "'+'", "'-'", "'.'", "'T'", "'['", 
			"'('", "')'", "'^'", "'V'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			"Identifier"
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

	@Override
	public String getGrammarFileName() { return "JvmSignature.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public JvmSignatureParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class AllContext extends ParserRuleContext {
		public FieldSignatureContext fieldSignature() {
			return getRuleContext(FieldSignatureContext.class,0);
		}
		public MethodSignatureContext methodSignature() {
			return getRuleContext(MethodSignatureContext.class,0);
		}
		public ClassSignatureContext classSignature() {
			return getRuleContext(ClassSignatureContext.class,0);
		}
		public AllContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_all; }
	}

	public final AllContext all() throws RecognitionException {
		AllContext _localctx = new AllContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_all);
		try {
			setState(53);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,0,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(50);
				fieldSignature();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(51);
				methodSignature();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(52);
				classSignature();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class JavaTypeSignatureContext extends ParserRuleContext {
		public ReferenceTypeSignatureContext referenceTypeSignature() {
			return getRuleContext(ReferenceTypeSignatureContext.class,0);
		}
		public BaseTypeContext baseType() {
			return getRuleContext(BaseTypeContext.class,0);
		}
		public JavaTypeSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_javaTypeSignature; }
	}

	public final JavaTypeSignatureContext javaTypeSignature() throws RecognitionException {
		JavaTypeSignatureContext _localctx = new JavaTypeSignatureContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_javaTypeSignature);
		try {
			setState(57);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__8:
			case T__17:
			case T__18:
				enterOuterAlt(_localctx, 1);
				{
				setState(55);
				referenceTypeSignature();
				}
				break;
			case T__0:
			case T__1:
			case T__2:
			case T__3:
			case T__4:
			case T__5:
			case T__6:
			case T__7:
				enterOuterAlt(_localctx, 2);
				{
				setState(56);
				baseType();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BaseTypeContext extends ParserRuleContext {
		public BaseTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_baseType; }
	}

	public final BaseTypeContext baseType() throws RecognitionException {
		BaseTypeContext _localctx = new BaseTypeContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_baseType);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(59);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__0) | (1L << T__1) | (1L << T__2) | (1L << T__3) | (1L << T__4) | (1L << T__5) | (1L << T__6) | (1L << T__7))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ReferenceTypeSignatureContext extends ParserRuleContext {
		public ClassTypeSignatureContext classTypeSignature() {
			return getRuleContext(ClassTypeSignatureContext.class,0);
		}
		public TypeVariableSignatureContext typeVariableSignature() {
			return getRuleContext(TypeVariableSignatureContext.class,0);
		}
		public ArrayTypeSignatureContext arrayTypeSignature() {
			return getRuleContext(ArrayTypeSignatureContext.class,0);
		}
		public ReferenceTypeSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_referenceTypeSignature; }
	}

	public final ReferenceTypeSignatureContext referenceTypeSignature() throws RecognitionException {
		ReferenceTypeSignatureContext _localctx = new ReferenceTypeSignatureContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_referenceTypeSignature);
		try {
			setState(64);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__8:
				enterOuterAlt(_localctx, 1);
				{
				setState(61);
				classTypeSignature();
				}
				break;
			case T__17:
				enterOuterAlt(_localctx, 2);
				{
				setState(62);
				typeVariableSignature();
				}
				break;
			case T__18:
				enterOuterAlt(_localctx, 3);
				{
				setState(63);
				arrayTypeSignature();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ClassTypeSignatureContext extends ParserRuleContext {
		public SimpleClassTypeSignatureContext simpleClassTypeSignature() {
			return getRuleContext(SimpleClassTypeSignatureContext.class,0);
		}
		public PackageSpecifierContext packageSpecifier() {
			return getRuleContext(PackageSpecifierContext.class,0);
		}
		public List<ClassTypeSignatureSuffixContext> classTypeSignatureSuffix() {
			return getRuleContexts(ClassTypeSignatureSuffixContext.class);
		}
		public ClassTypeSignatureSuffixContext classTypeSignatureSuffix(int i) {
			return getRuleContext(ClassTypeSignatureSuffixContext.class,i);
		}
		public ClassTypeSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_classTypeSignature; }
	}

	public final ClassTypeSignatureContext classTypeSignature() throws RecognitionException {
		ClassTypeSignatureContext _localctx = new ClassTypeSignatureContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_classTypeSignature);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(66);
			match(T__8);
			setState(68);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,3,_ctx) ) {
			case 1:
				{
				setState(67);
				packageSpecifier();
				}
				break;
			}
			setState(70);
			simpleClassTypeSignature();
			setState(74);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__16) {
				{
				{
				setState(71);
				classTypeSignatureSuffix();
				}
				}
				setState(76);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(77);
			match(T__9);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PackageSpecifierContext extends ParserRuleContext {
		public TerminalNode Identifier() { return getToken(JvmSignatureParser.Identifier, 0); }
		public PackageSpecifierContext packageSpecifier() {
			return getRuleContext(PackageSpecifierContext.class,0);
		}
		public PackageSpecifierContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_packageSpecifier; }
	}

	public final PackageSpecifierContext packageSpecifier() throws RecognitionException {
		PackageSpecifierContext _localctx = new PackageSpecifierContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_packageSpecifier);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(79);
			match(Identifier);
			setState(80);
			match(T__10);
			setState(81);
			packageSpecifier();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class SimpleClassTypeSignatureContext extends ParserRuleContext {
		public TerminalNode Identifier() { return getToken(JvmSignatureParser.Identifier, 0); }
		public TypeArgumentsContext typeArguments() {
			return getRuleContext(TypeArgumentsContext.class,0);
		}
		public SimpleClassTypeSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_simpleClassTypeSignature; }
	}

	public final SimpleClassTypeSignatureContext simpleClassTypeSignature() throws RecognitionException {
		SimpleClassTypeSignatureContext _localctx = new SimpleClassTypeSignatureContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_simpleClassTypeSignature);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(83);
			match(Identifier);
			setState(85);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__11) {
				{
				setState(84);
				typeArguments();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TypeArgumentsContext extends ParserRuleContext {
		public List<TypeArgumentContext> typeArgument() {
			return getRuleContexts(TypeArgumentContext.class);
		}
		public TypeArgumentContext typeArgument(int i) {
			return getRuleContext(TypeArgumentContext.class,i);
		}
		public TypeArgumentsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typeArguments; }
	}

	public final TypeArgumentsContext typeArguments() throws RecognitionException {
		TypeArgumentsContext _localctx = new TypeArgumentsContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_typeArguments);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(87);
			match(T__11);
			setState(88);
			typeArgument();
			setState(92);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__8) | (1L << T__14) | (1L << T__15) | (1L << T__17) | (1L << T__18))) != 0)) {
				{
				{
				setState(89);
				typeArgument();
				}
				}
				setState(94);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(95);
			match(T__12);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TypeArgumentContext extends ParserRuleContext {
		public ReferenceTypeSignatureContext referenceTypeSignature() {
			return getRuleContext(ReferenceTypeSignatureContext.class,0);
		}
		public WildcardIndicatorContext wildcardIndicator() {
			return getRuleContext(WildcardIndicatorContext.class,0);
		}
		public TypeArgumentContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typeArgument; }
	}

	public final TypeArgumentContext typeArgument() throws RecognitionException {
		TypeArgumentContext _localctx = new TypeArgumentContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_typeArgument);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(98);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__14 || _la==T__15) {
				{
				setState(97);
				wildcardIndicator();
				}
			}

			setState(100);
			referenceTypeSignature();
			setState(101);
			match(T__13);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class WildcardIndicatorContext extends ParserRuleContext {
		public WildcardIndicatorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_wildcardIndicator; }
	}

	public final WildcardIndicatorContext wildcardIndicator() throws RecognitionException {
		WildcardIndicatorContext _localctx = new WildcardIndicatorContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_wildcardIndicator);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(103);
			_la = _input.LA(1);
			if ( !(_la==T__14 || _la==T__15) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ClassTypeSignatureSuffixContext extends ParserRuleContext {
		public SimpleClassTypeSignatureContext simpleClassTypeSignature() {
			return getRuleContext(SimpleClassTypeSignatureContext.class,0);
		}
		public ClassTypeSignatureSuffixContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_classTypeSignatureSuffix; }
	}

	public final ClassTypeSignatureSuffixContext classTypeSignatureSuffix() throws RecognitionException {
		ClassTypeSignatureSuffixContext _localctx = new ClassTypeSignatureSuffixContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_classTypeSignatureSuffix);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(105);
			match(T__16);
			setState(106);
			simpleClassTypeSignature();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TypeVariableSignatureContext extends ParserRuleContext {
		public TerminalNode Identifier() { return getToken(JvmSignatureParser.Identifier, 0); }
		public TypeVariableSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typeVariableSignature; }
	}

	public final TypeVariableSignatureContext typeVariableSignature() throws RecognitionException {
		TypeVariableSignatureContext _localctx = new TypeVariableSignatureContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_typeVariableSignature);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(108);
			match(T__17);
			setState(109);
			match(Identifier);
			setState(110);
			match(T__9);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ArrayTypeSignatureContext extends ParserRuleContext {
		public JavaTypeSignatureContext javaTypeSignature() {
			return getRuleContext(JavaTypeSignatureContext.class,0);
		}
		public ArrayTypeSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arrayTypeSignature; }
	}

	public final ArrayTypeSignatureContext arrayTypeSignature() throws RecognitionException {
		ArrayTypeSignatureContext _localctx = new ArrayTypeSignatureContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_arrayTypeSignature);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(112);
			match(T__18);
			setState(113);
			javaTypeSignature();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ClassSignatureContext extends ParserRuleContext {
		public SuperclassSignatureContext superclassSignature() {
			return getRuleContext(SuperclassSignatureContext.class,0);
		}
		public TerminalNode EOF() { return getToken(JvmSignatureParser.EOF, 0); }
		public TypeParametersContext typeParameters() {
			return getRuleContext(TypeParametersContext.class,0);
		}
		public List<SuperInterfaceSignatureContext> superInterfaceSignature() {
			return getRuleContexts(SuperInterfaceSignatureContext.class);
		}
		public SuperInterfaceSignatureContext superInterfaceSignature(int i) {
			return getRuleContext(SuperInterfaceSignatureContext.class,i);
		}
		public ClassSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_classSignature; }
	}

	public final ClassSignatureContext classSignature() throws RecognitionException {
		ClassSignatureContext _localctx = new ClassSignatureContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_classSignature);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(116);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__11) {
				{
				setState(115);
				typeParameters();
				}
			}

			setState(118);
			superclassSignature();
			setState(122);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__8) {
				{
				{
				setState(119);
				superInterfaceSignature();
				}
				}
				setState(124);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(125);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TypeParametersContext extends ParserRuleContext {
		public List<TypeParameterContext> typeParameter() {
			return getRuleContexts(TypeParameterContext.class);
		}
		public TypeParameterContext typeParameter(int i) {
			return getRuleContext(TypeParameterContext.class,i);
		}
		public TypeParametersContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typeParameters; }
	}

	public final TypeParametersContext typeParameters() throws RecognitionException {
		TypeParametersContext _localctx = new TypeParametersContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_typeParameters);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(127);
			match(T__11);
			setState(128);
			typeParameter();
			setState(132);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==Identifier) {
				{
				{
				setState(129);
				typeParameter();
				}
				}
				setState(134);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(135);
			match(T__12);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TypeParameterContext extends ParserRuleContext {
		public TerminalNode Identifier() { return getToken(JvmSignatureParser.Identifier, 0); }
		public ClassBoundContext classBound() {
			return getRuleContext(ClassBoundContext.class,0);
		}
		public List<InterfaceBoundContext> interfaceBound() {
			return getRuleContexts(InterfaceBoundContext.class);
		}
		public InterfaceBoundContext interfaceBound(int i) {
			return getRuleContext(InterfaceBoundContext.class,i);
		}
		public TypeParameterContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typeParameter; }
	}

	public final TypeParameterContext typeParameter() throws RecognitionException {
		TypeParameterContext _localctx = new TypeParameterContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_typeParameter);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(137);
			match(Identifier);
			setState(138);
			classBound();
			setState(142);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__8) | (1L << T__17) | (1L << T__18))) != 0)) {
				{
				{
				setState(139);
				interfaceBound();
				}
				}
				setState(144);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ClassBoundContext extends ParserRuleContext {
		public ReferenceTypeSignatureContext referenceTypeSignature() {
			return getRuleContext(ReferenceTypeSignatureContext.class,0);
		}
		public ClassBoundContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_classBound; }
	}

	public final ClassBoundContext classBound() throws RecognitionException {
		ClassBoundContext _localctx = new ClassBoundContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_classBound);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(146);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,12,_ctx) ) {
			case 1:
				{
				setState(145);
				referenceTypeSignature();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class SuperclassSignatureContext extends ParserRuleContext {
		public ClassTypeSignatureContext classTypeSignature() {
			return getRuleContext(ClassTypeSignatureContext.class,0);
		}
		public SuperclassSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_superclassSignature; }
	}

	public final SuperclassSignatureContext superclassSignature() throws RecognitionException {
		SuperclassSignatureContext _localctx = new SuperclassSignatureContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_superclassSignature);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(148);
			classTypeSignature();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class SuperInterfaceSignatureContext extends ParserRuleContext {
		public ClassTypeSignatureContext classTypeSignature() {
			return getRuleContext(ClassTypeSignatureContext.class,0);
		}
		public SuperInterfaceSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_superInterfaceSignature; }
	}

	public final SuperInterfaceSignatureContext superInterfaceSignature() throws RecognitionException {
		SuperInterfaceSignatureContext _localctx = new SuperInterfaceSignatureContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_superInterfaceSignature);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(150);
			classTypeSignature();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class InterfaceBoundContext extends ParserRuleContext {
		public ReferenceTypeSignatureContext referenceTypeSignature() {
			return getRuleContext(ReferenceTypeSignatureContext.class,0);
		}
		public InterfaceBoundContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_interfaceBound; }
	}

	public final InterfaceBoundContext interfaceBound() throws RecognitionException {
		InterfaceBoundContext _localctx = new InterfaceBoundContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_interfaceBound);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(152);
			referenceTypeSignature();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MethodSignatureContext extends ParserRuleContext {
		public ResultContext result() {
			return getRuleContext(ResultContext.class,0);
		}
		public TerminalNode EOF() { return getToken(JvmSignatureParser.EOF, 0); }
		public TypeParameterContext typeParameter() {
			return getRuleContext(TypeParameterContext.class,0);
		}
		public List<JavaTypeSignatureContext> javaTypeSignature() {
			return getRuleContexts(JavaTypeSignatureContext.class);
		}
		public JavaTypeSignatureContext javaTypeSignature(int i) {
			return getRuleContext(JavaTypeSignatureContext.class,i);
		}
		public List<ThrowsSignatureContext> throwsSignature() {
			return getRuleContexts(ThrowsSignatureContext.class);
		}
		public ThrowsSignatureContext throwsSignature(int i) {
			return getRuleContext(ThrowsSignatureContext.class,i);
		}
		public MethodSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_methodSignature; }
	}

	public final MethodSignatureContext methodSignature() throws RecognitionException {
		MethodSignatureContext _localctx = new MethodSignatureContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_methodSignature);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(155);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==Identifier) {
				{
				setState(154);
				typeParameter();
				}
			}

			setState(157);
			match(T__19);
			setState(161);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__0) | (1L << T__1) | (1L << T__2) | (1L << T__3) | (1L << T__4) | (1L << T__5) | (1L << T__6) | (1L << T__7) | (1L << T__8) | (1L << T__17) | (1L << T__18))) != 0)) {
				{
				{
				setState(158);
				javaTypeSignature();
				}
				}
				setState(163);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(164);
			match(T__20);
			setState(165);
			result();
			setState(169);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__21) {
				{
				{
				setState(166);
				throwsSignature();
				}
				}
				setState(171);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(172);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ResultContext extends ParserRuleContext {
		public JavaTypeSignatureContext javaTypeSignature() {
			return getRuleContext(JavaTypeSignatureContext.class,0);
		}
		public VoidDescriptorContext voidDescriptor() {
			return getRuleContext(VoidDescriptorContext.class,0);
		}
		public ResultContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_result; }
	}

	public final ResultContext result() throws RecognitionException {
		ResultContext _localctx = new ResultContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_result);
		try {
			setState(176);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__0:
			case T__1:
			case T__2:
			case T__3:
			case T__4:
			case T__5:
			case T__6:
			case T__7:
			case T__8:
			case T__17:
			case T__18:
				enterOuterAlt(_localctx, 1);
				{
				setState(174);
				javaTypeSignature();
				}
				break;
			case T__22:
				enterOuterAlt(_localctx, 2);
				{
				setState(175);
				voidDescriptor();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ThrowsSignatureContext extends ParserRuleContext {
		public ClassTypeSignatureContext classTypeSignature() {
			return getRuleContext(ClassTypeSignatureContext.class,0);
		}
		public TypeVariableSignatureContext typeVariableSignature() {
			return getRuleContext(TypeVariableSignatureContext.class,0);
		}
		public ThrowsSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_throwsSignature; }
	}

	public final ThrowsSignatureContext throwsSignature() throws RecognitionException {
		ThrowsSignatureContext _localctx = new ThrowsSignatureContext(_ctx, getState());
		enterRule(_localctx, 44, RULE_throwsSignature);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(178);
			match(T__21);
			setState(179);
			classTypeSignature();
			setState(180);
			match(T__21);
			setState(181);
			typeVariableSignature();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class VoidDescriptorContext extends ParserRuleContext {
		public VoidDescriptorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_voidDescriptor; }
	}

	public final VoidDescriptorContext voidDescriptor() throws RecognitionException {
		VoidDescriptorContext _localctx = new VoidDescriptorContext(_ctx, getState());
		enterRule(_localctx, 46, RULE_voidDescriptor);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(183);
			match(T__22);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FieldSignatureContext extends ParserRuleContext {
		public ReferenceTypeSignatureContext referenceTypeSignature() {
			return getRuleContext(ReferenceTypeSignatureContext.class,0);
		}
		public TerminalNode EOF() { return getToken(JvmSignatureParser.EOF, 0); }
		public FieldSignatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fieldSignature; }
	}

	public final FieldSignatureContext fieldSignature() throws RecognitionException {
		FieldSignatureContext _localctx = new FieldSignatureContext(_ctx, getState());
		enterRule(_localctx, 48, RULE_fieldSignature);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(185);
			referenceTypeSignature();
			setState(186);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3\32\u00bf\4\2\t\2"+
		"\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\3\2\3\2\3\2\5\28\n\2\3\3\3\3\5\3<\n\3\3\4\3\4\3\5\3\5\3\5\5"+
		"\5C\n\5\3\6\3\6\5\6G\n\6\3\6\3\6\7\6K\n\6\f\6\16\6N\13\6\3\6\3\6\3\7\3"+
		"\7\3\7\3\7\3\b\3\b\5\bX\n\b\3\t\3\t\3\t\7\t]\n\t\f\t\16\t`\13\t\3\t\3"+
		"\t\3\n\5\ne\n\n\3\n\3\n\3\n\3\13\3\13\3\f\3\f\3\f\3\r\3\r\3\r\3\r\3\16"+
		"\3\16\3\16\3\17\5\17w\n\17\3\17\3\17\7\17{\n\17\f\17\16\17~\13\17\3\17"+
		"\3\17\3\20\3\20\3\20\7\20\u0085\n\20\f\20\16\20\u0088\13\20\3\20\3\20"+
		"\3\21\3\21\3\21\7\21\u008f\n\21\f\21\16\21\u0092\13\21\3\22\5\22\u0095"+
		"\n\22\3\23\3\23\3\24\3\24\3\25\3\25\3\26\5\26\u009e\n\26\3\26\3\26\7\26"+
		"\u00a2\n\26\f\26\16\26\u00a5\13\26\3\26\3\26\3\26\7\26\u00aa\n\26\f\26"+
		"\16\26\u00ad\13\26\3\26\3\26\3\27\3\27\5\27\u00b3\n\27\3\30\3\30\3\30"+
		"\3\30\3\30\3\31\3\31\3\32\3\32\3\32\3\32\2\2\33\2\4\6\b\n\f\16\20\22\24"+
		"\26\30\32\34\36 \"$&(*,.\60\62\2\4\3\2\3\n\3\2\21\22\2\u00b8\2\67\3\2"+
		"\2\2\4;\3\2\2\2\6=\3\2\2\2\bB\3\2\2\2\nD\3\2\2\2\fQ\3\2\2\2\16U\3\2\2"+
		"\2\20Y\3\2\2\2\22d\3\2\2\2\24i\3\2\2\2\26k\3\2\2\2\30n\3\2\2\2\32r\3\2"+
		"\2\2\34v\3\2\2\2\36\u0081\3\2\2\2 \u008b\3\2\2\2\"\u0094\3\2\2\2$\u0096"+
		"\3\2\2\2&\u0098\3\2\2\2(\u009a\3\2\2\2*\u009d\3\2\2\2,\u00b2\3\2\2\2."+
		"\u00b4\3\2\2\2\60\u00b9\3\2\2\2\62\u00bb\3\2\2\2\648\5\62\32\2\658\5*"+
		"\26\2\668\5\34\17\2\67\64\3\2\2\2\67\65\3\2\2\2\67\66\3\2\2\28\3\3\2\2"+
		"\29<\5\b\5\2:<\5\6\4\2;9\3\2\2\2;:\3\2\2\2<\5\3\2\2\2=>\t\2\2\2>\7\3\2"+
		"\2\2?C\5\n\6\2@C\5\30\r\2AC\5\32\16\2B?\3\2\2\2B@\3\2\2\2BA\3\2\2\2C\t"+
		"\3\2\2\2DF\7\13\2\2EG\5\f\7\2FE\3\2\2\2FG\3\2\2\2GH\3\2\2\2HL\5\16\b\2"+
		"IK\5\26\f\2JI\3\2\2\2KN\3\2\2\2LJ\3\2\2\2LM\3\2\2\2MO\3\2\2\2NL\3\2\2"+
		"\2OP\7\f\2\2P\13\3\2\2\2QR\7\32\2\2RS\7\r\2\2ST\5\f\7\2T\r\3\2\2\2UW\7"+
		"\32\2\2VX\5\20\t\2WV\3\2\2\2WX\3\2\2\2X\17\3\2\2\2YZ\7\16\2\2Z^\5\22\n"+
		"\2[]\5\22\n\2\\[\3\2\2\2]`\3\2\2\2^\\\3\2\2\2^_\3\2\2\2_a\3\2\2\2`^\3"+
		"\2\2\2ab\7\17\2\2b\21\3\2\2\2ce\5\24\13\2dc\3\2\2\2de\3\2\2\2ef\3\2\2"+
		"\2fg\5\b\5\2gh\7\20\2\2h\23\3\2\2\2ij\t\3\2\2j\25\3\2\2\2kl\7\23\2\2l"+
		"m\5\16\b\2m\27\3\2\2\2no\7\24\2\2op\7\32\2\2pq\7\f\2\2q\31\3\2\2\2rs\7"+
		"\25\2\2st\5\4\3\2t\33\3\2\2\2uw\5\36\20\2vu\3\2\2\2vw\3\2\2\2wx\3\2\2"+
		"\2x|\5$\23\2y{\5&\24\2zy\3\2\2\2{~\3\2\2\2|z\3\2\2\2|}\3\2\2\2}\177\3"+
		"\2\2\2~|\3\2\2\2\177\u0080\7\2\2\3\u0080\35\3\2\2\2\u0081\u0082\7\16\2"+
		"\2\u0082\u0086\5 \21\2\u0083\u0085\5 \21\2\u0084\u0083\3\2\2\2\u0085\u0088"+
		"\3\2\2\2\u0086\u0084\3\2\2\2\u0086\u0087\3\2\2\2\u0087\u0089\3\2\2\2\u0088"+
		"\u0086\3\2\2\2\u0089\u008a\7\17\2\2\u008a\37\3\2\2\2\u008b\u008c\7\32"+
		"\2\2\u008c\u0090\5\"\22\2\u008d\u008f\5(\25\2\u008e\u008d\3\2\2\2\u008f"+
		"\u0092\3\2\2\2\u0090\u008e\3\2\2\2\u0090\u0091\3\2\2\2\u0091!\3\2\2\2"+
		"\u0092\u0090\3\2\2\2\u0093\u0095\5\b\5\2\u0094\u0093\3\2\2\2\u0094\u0095"+
		"\3\2\2\2\u0095#\3\2\2\2\u0096\u0097\5\n\6\2\u0097%\3\2\2\2\u0098\u0099"+
		"\5\n\6\2\u0099\'\3\2\2\2\u009a\u009b\5\b\5\2\u009b)\3\2\2\2\u009c\u009e"+
		"\5 \21\2\u009d\u009c\3\2\2\2\u009d\u009e\3\2\2\2\u009e\u009f\3\2\2\2\u009f"+
		"\u00a3\7\26\2\2\u00a0\u00a2\5\4\3\2\u00a1\u00a0\3\2\2\2\u00a2\u00a5\3"+
		"\2\2\2\u00a3\u00a1\3\2\2\2\u00a3\u00a4\3\2\2\2\u00a4\u00a6\3\2\2\2\u00a5"+
		"\u00a3\3\2\2\2\u00a6\u00a7\7\27\2\2\u00a7\u00ab\5,\27\2\u00a8\u00aa\5"+
		".\30\2\u00a9\u00a8\3\2\2\2\u00aa\u00ad\3\2\2\2\u00ab\u00a9\3\2\2\2\u00ab"+
		"\u00ac\3\2\2\2\u00ac\u00ae\3\2\2\2\u00ad\u00ab\3\2\2\2\u00ae\u00af\7\2"+
		"\2\3\u00af+\3\2\2\2\u00b0\u00b3\5\4\3\2\u00b1\u00b3\5\60\31\2\u00b2\u00b0"+
		"\3\2\2\2\u00b2\u00b1\3\2\2\2\u00b3-\3\2\2\2\u00b4\u00b5\7\30\2\2\u00b5"+
		"\u00b6\5\n\6\2\u00b6\u00b7\7\30\2\2\u00b7\u00b8\5\30\r\2\u00b8/\3\2\2"+
		"\2\u00b9\u00ba\7\31\2\2\u00ba\61\3\2\2\2\u00bb\u00bc\5\b\5\2\u00bc\u00bd"+
		"\7\2\2\3\u00bd\63\3\2\2\2\23\67;BFLW^dv|\u0086\u0090\u0094\u009d\u00a3"+
		"\u00ab\u00b2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}