/** @type {import('jest').Config} */
module.exports = {
  testEnvironment: 'node',
  modulePathIgnorePatterns: ['<rootDir>/example/', '<rootDir>/lib/'],
  transform: {
    '^.+\\.(ts|tsx)$': [
      'ts-jest',
      {
        tsconfig: {
          module: 'CommonJS',
          moduleResolution: 'node',
          esModuleInterop: true,
          strict: true,
          skipLibCheck: true,
          noEmit: false,
          verbatimModuleSyntax: false,
          noUnusedLocals: false,
          noUnusedParameters: false,
          types: ['jest', 'node'],
        },
      },
    ],
  },
};
