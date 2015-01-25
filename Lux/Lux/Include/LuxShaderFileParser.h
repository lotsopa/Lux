#ifndef LUX_SHADER_FILE_PARSER
#define LUX_SHADER_FILE_PARSER

namespace Lux
{
	namespace Core
	{
		class Key;

		class ShaderFileParser
		{
		public:
			ShaderFileParser();
			~ShaderFileParser();

			void ProcessIdentifier(Key& a_Key);
			void ProcessString(Key& a_Key);
			void ProcessOpenBrace();
			void ProcessCloseBrace();
			void ProcessEquals();
			void ProcessSemiColon();

			const String& GetParsedProgramGLSL(ShaderProgram a_ProgramType);
			const String& GetParsedProgramHLSL(ShaderProgram a_ProgramType);

			enum State
			{
				STATE_BASE = 0,
				STATE_SHADER,
				STATE_GLSL,
				STATE_HLSL,
				STATE_VERTEX_PROGRAM,
				STATE_FRAGMENT_PROGRAM,
				STATE_GEOMETRY_PROGRAM
			};
		protected:
			State m_State;
			State m_PlatformState;
			String m_GLSLPrograms[NUM_SHADER_PROGRAMS];
			String m_HLSLPrograms[NUM_SHADER_PROGRAMS];
		};
	}
}

#endif